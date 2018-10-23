#include    "motor.h"

/** \file
    \brief Digital differential analyser - this is where we figure out which steppers need to move, and when they need to move
*/

#include    <string.h>
#include    <stdlib.h>
#include    <math.h>

#include    "maths.h"
#include    "kinematics.h"
#include    "timer.h"
#include    "serial.h"
#include    "queue.h"

#include    "pinio.h"
#include    "sensors_control.h"

/*
    position tracking
*/

/// \var startpoint
/// \brief target position of last move in queue
TARGET BSS startpoint;

/// \var startpoint_steps
/// \brief target position of last move in queue, expressed in steps
TARGET BSS startpoint_steps;

/// \var steps_per_m_P
/// \brief motor steps required to advance one meter on each axis
static const axes_uint32_t PROGMEM steps_per_m_P = {
  STEPS_PER_M_X,
  STEPS_PER_M_Y
  #ifdef STEPS_PER_M_Z
    ,STEPS_PER_M_Z
  #endif
  #ifdef STEPS_PER_M_E
    ,STEPS_PER_M_E
  #endif
};

/// \var maximum_feedrate_P
/// \brief maximum allowed feedrate on each axis
static const axes_uint32_t PROGMEM maximum_feedrate_P = {
  MAXIMUM_FEEDRATE_X,
  MAXIMUM_FEEDRATE_Y
  #ifdef MAXIMUM_FEEDRATE_Z
    ,MAXIMUM_FEEDRATE_Z
  #endif
  #ifdef MAXIMUM_FEEDRATE_E
    ,MAXIMUM_FEEDRATE_E
  #endif
};

/// \var current_position
/// \brief actual position of carriage
/// \todo make current_position = real_position (from endstops) + offset from G28 and friends
TARGET BSS current_position;

/// \var move_state
/// \brief numbers for tracking the current state of movement
MOVE_STATE BSS move_state;

/// \var c0_P
/// \brief Initialization constant for the ramping algorithm. Timer cycles for
///        first step interval.
static const axes_uint32_t PROGMEM c0_P = {
  (uint32_t)((double)F_CPU / SQRT((double)STEPS_PER_M_X * ACCELERATION / 2000.)),
  (uint32_t)((double)F_CPU / SQRT((double)STEPS_PER_M_Y * ACCELERATION / 2000.))
  #ifdef STEPS_PER_M_Z
  ,(uint32_t)((double)F_CPU / SQRT((double)STEPS_PER_M_Z * ACCELERATION / 2000.))
  #endif
  #ifdef STEPS_PER_M_E
  ,(uint32_t)((double)F_CPU / SQRT((double)STEPS_PER_M_E * ACCELERATION / 2000.))
  #endif
};

/*! Inititalise DDA movement structures
*/
void dda_init(void) {
    // set up default feedrate
    if (startpoint.F == 0)
        startpoint.F = SEARCH_FEEDRATE_Y;
  #ifdef STEPS_PER_M_E
    if (startpoint.e_multiplier == 0)
    startpoint.e_multiplier =  256;
#endif
  if (startpoint.f_multiplier == 0)
    startpoint.f_multiplier = 256;
}

/*! Distribute a new startpoint to DDA's internal structures without any movement.

    This is needed for example after homing or a G92. The new location must be in startpoint already.
*/
void dda_new_startpoint(void) {
    axes_um_to_steps(startpoint.axis, startpoint_steps.axis);
}

/*! Set the direction of the 'n' axis
*/
static void set_direction(DDA *dda, uint8_t n, int32_t delta) {
  uint8_t dir = (delta >= 0) ? 1 : 0;

  if (n == X)
    dda->x_direction = dir;
  else if (n == Y)
    dda->y_direction = dir;
#ifdef STEPS_PER_M_Z
  else if (n == Z)
    dda->z_direction = dir;
#endif
#ifdef STEPS_PER_M_E
  else if (n == E)
    dda->e_direction = dir;
#endif
}

/*! Find the direction of the 'n' axis
*/
static int8_t get_direction(DDA *dda, uint8_t n) {
  if ((n == X && dda->x_direction) ||
      (n == Y && dda->y_direction) 
#ifdef STEPS_PER_M_Z
      || (n == Z && dda->z_direction) 
#endif
#ifdef STEPS_PER_M_E
      || (n == E && dda->e_direction)
#endif
    )
    return 1;
  else
    return -1;
}

void dda_create(DDA *dda, const TARGET *target) {
  axes_uint32_t delta_um;
  axes_int32_t steps;
    uint32_t    distance, c_limit, c_limit_calc;
  uint8_t i;
  static DDA* prev_dda = NULL;
  #ifdef LOOKAHEAD
  // Number the moves to identify them; allowed to overflow.
  static uint8_t idcnt = 0;

  if (prev_dda && prev_dda->done)
    prev_dda = NULL;
  #endif

  // We end at the passed target.
  memcpy(&(dda->endpoint), target, sizeof(TARGET));
#ifdef STEPS_PER_M_Z
    if (DEBUG_DDA && (debug_flags & DEBUG_DDA))
        sersendf_P(PSTR("\nCreate: X %lq  Y %lq  Z %lq  F %lu\n"),
               dda->endpoint.axis[X], dda->endpoint.axis[Y],
               dda->endpoint.axis[Z], dda->endpoint.F );
#else
    sersendf_P(PSTR("\nCreate: X %lq  Y %lq  F %lu\n"),
               dda->endpoint.axis[X], dda->endpoint.axis[Y], dda->endpoint.F );
#endif
             

  // Apply feedrate multiplier.
  if (dda->endpoint.f_multiplier != 256) {
    dda->endpoint.F *= dda->endpoint.f_multiplier;
    dda->endpoint.F += 128;
    dda->endpoint.F /= 256;
  }

  #ifdef LOOKAHEAD
    // Set the start and stop speeds to zero for now = full stops between
    // moves. Also fallback if lookahead calculations fail to finish in time.
    dda->crossF = 0;
    dda->start_steps = 0;
    dda->end_steps = 0;
    // Give this move an identifier.
    dda->id = idcnt++;
  #endif

  // Handle bot axes. They're subject to kinematics considerations.
  code_axes_to_stepper_axes(&startpoint, target, delta_um, steps);
#ifdef STEPS_PER_M_Z
  for (i = X; i <= Z; i++) {
#else
  for (i = X; i <= Y; i++) {
#endif
    int32_t delta_steps;

    delta_steps = steps[i] - startpoint_steps.axis[i];
    dda->delta[i] = (uint32_t)labs(delta_steps);
    startpoint_steps.axis[i] = steps[i];

    set_direction(dda, i, delta_steps);
    #ifdef LOOKAHEAD
      // Also displacements in micrometers, but for the lookahead alogrithms.
      // TODO: this is redundant. delta_um[] and dda->delta_um[] differ by
      //       just signedness and storage location. Ideally, dda is used
      //       as storage place only if neccessary (LOOKAHEAD turned on?)
      //       because this space is multiplied by the movement queue size.
      //
      // Update 2014/10: it was tried to use delta_um[]'s sign to set stepper
      //                 direction in dda_start() to allow getting rid of
      //                 some of this redundancy, but this increases dda_start()
      //                 by at least 20 clock cycles. Not good for performance.
      //                 Tried code can be found in the archive folder.
      dda->delta_um[i] = (delta_steps >= 0) ?
                         (int32_t)delta_um[i] : -(int32_t)delta_um[i];
    #endif
  }

#ifdef STEPS_PER_M_E
  // Handle extruder axes. They act independently from the bots kinematics
  // type, but are subject to other special handling.
  steps[E] = um_to_steps(target->axis[E], E);

  // Apply extrusion multiplier.
  if (target->e_multiplier != 256) {
  steps[E] *= target->e_multiplier;
    steps[E] += 128;
    steps[E] /= 256;
  }

  if ( ! target->e_relative) {
    int32_t delta_steps;

    delta_um[E] = (uint32_t)labs(target->axis[E] - startpoint.axis[E]);
    delta_steps = steps[E] - startpoint_steps.axis[E];
    dda->delta[E] = (uint32_t)labs(delta_steps);
    startpoint_steps.axis[E] = steps[E];

    set_direction(dda, E, delta_steps);
    #ifdef LOOKAHEAD
      // Also displacements in micrometers, but for the lookahead alogrithms.
      // TODO: this is redundant. delta_um[] and dda->delta_um[] differ by
      //       just signedness and storage location. Ideally, dda is used
      //       as storage place only if neccessary (LOOKAHEAD turned on?)
      //       because this space is multiplied by the movement queue size.
      dda->delta_um[E] = (delta_steps >= 0) ?
                         (int32_t)delta_um[E] : -(int32_t)delta_um[E];
    #endif
  }
  else {
    // When we get more extruder axes:
    // for (i = E; i < AXIS_COUNT; i++) { ...
    delta_um[E] = (uint32_t)labs(target->axis[E]);
    dda->delta[E] = (uint32_t)labs(steps[E]);
    #ifdef LOOKAHEAD
      dda->delta_um[E] = target->axis[E];
    #endif
    dda->e_direction = (target->axis[E] >= 0)?1:0;
    }
#endif
    if (DEBUG_DDA && (debug_flags & DEBUG_DDA))
    sersendf_P(PSTR("[%ld,%ld,%ld,%ld]"),
               target->axis[X] - startpoint.axis[X], target->axis[Y] - startpoint.axis[Y],
             #ifdef STEPS_PER_M_Z
               target->axis[Z] - startpoint.axis[Z], 
             #else
               0,
             #endif
             #ifdef STEPS_PER_M_E
               target->axis[E] - startpoint.axis[E]
             #else
               0
             #endif
              );

  // Admittedly, this looks like it's overcomplicated. Why store three 32-bit
  // values if storing an axis number would be fully sufficient? Well, I'm not
  // sure, but my feeling says that when we achieve true circles and Beziers,
  // we'll have total_steps which matches neither of X, Y, Z or E. Accordingly,
  // keep it for now. --Traumflug
  for (i = X; i < AXIS_COUNT; i++) {
    if (i == X || dda->delta[i] > dda->total_steps) {
      dda->fast_axis = i;
      dda->total_steps = dda->delta[i];
      dda->fast_um = delta_um[i];
      dda->fast_spm = pgm_read_dword(&steps_per_m_P[i]);
    }
  }

    if (DEBUG_DDA && (debug_flags & DEBUG_DDA))
        sersendf_P(PSTR(" [ts:%lu"), dda->total_steps);

    // null moves should be accepted anyway
    //if (dda->total_steps == 0) {
    //    dda->nullmove = 1;
    //}
    //else 
    {
        // get steppers ready to go
        //power_on();
        stepper_enable();
        x_enable();
        y_enable();
#ifdef STEPS_PER_M_Z
        z_enable();
#endif
#ifdef STEPS_PER_M_E
        e_enable();
#endif
#ifdef STEPS_PER_M_Z
        // since it's unusual to combine X, Y and Z changes in a single move on reprap, check if we can use simpler approximations before trying the full 3d approximation.
        if (delta_um[Z] == 0)
            distance = approx_distance(delta_um[X], delta_um[Y]);
        else if (delta_um[X] == 0 && delta_um[Y] == 0)
            distance = delta_um[Z];
        else
            distance = approx_distance_3(delta_um[X], delta_um[Y], delta_um[Z]);
    #ifdef STEPS_PER_M_E
        if (distance < 2)
            distance = delta_um[E];
    #endif
#else
        distance = approx_distance(delta_um[X], delta_um[Y]);
#endif
        if (DEBUG_DDA && (debug_flags & DEBUG_DDA))
            sersendf_P(PSTR(",ds:%lu"), distance);


            // pre-calculate move speed in millimeter microseconds per step minute for less math in interrupt context
            // mm (distance) * 60000000 us/min / step (total_steps) = mm.us per step.min
            //   note: um (distance) * 60000 == mm * 60000000
            // so in the interrupt we must simply calculate
            // mm.us per step.min / mm per min (F) = us per step

            // break this calculation up a bit and lose some precision because 300,000um * 60000 is too big for a uint32
            // calculate this with a uint64 if you need the precision, but it'll take longer so routines with lots of short moves may suffer
            // 2^32/6000 is about 715mm which should be plenty

            // changed * 10 to * (F_CPU / 100000) so we can work in cpu_ticks rather than microseconds.
            // timer.c timer_set() routine altered for same reason

            // changed distance * 6000 .. * F_CPU / 100000 to
            //         distance * 2400 .. * F_CPU / 40000 so we can move a distance of up to 1800mm without overflowing
            uint32_t move_duration = ((distance * 2400) / dda->total_steps) * (F_CPU / 40000);

        // similarly, find out how fast we can run our axes.
        // do this for each axis individually, as the combined speed of two or more axes can be higher than the capabilities of a single one.
    // TODO: instead of calculating c_min directly, it's probably more simple
    //       to calculate (maximum) move_duration for each axis, like done for
    //       ACCELERATION_TEMPORAL above. This should make re-calculating the
    //       allowed F easier.
    c_limit = 0;
    for (i = X; i < AXIS_COUNT; i++) {
      c_limit_calc = (delta_um[i] * 2400L) /
                     dda->total_steps * (F_CPU / 40000) /
                     pgm_read_dword(&maximum_feedrate_P[i]);
      if (c_limit_calc > c_limit)
        c_limit = c_limit_calc;
    }

    dda->c_min = move_duration / dda->endpoint.F;
      if (dda->c_min < c_limit) {
        dda->c_min = c_limit;
        dda->endpoint.F = move_duration / dda->c_min;
      }

      // Lookahead can deal with 16 bits ( = 1092 mm/s), only.
      if (dda->endpoint.F > 65535)
        dda->endpoint.F = 65535;

      // Acceleration ramps are based on the fast axis, not the combined speed.
      dda->rampup_steps =
        acc_ramp_len(muldiv(dda->fast_um, dda->endpoint.F, distance),
                     dda->fast_spm);

      if (dda->rampup_steps > dda->total_steps / 2)
        dda->rampup_steps = dda->total_steps / 2;
      dda->rampdown_steps = dda->total_steps - dda->rampup_steps;

      #ifdef LOOKAHEAD
        dda->distance = distance;
        dda_find_crossing_speed(prev_dda, dda);
        // TODO: this should become a reverse-stepping through the existing
        //       movement queue to allow higher speeds for short moves.
        //       dda_find_crossing_speed() is required only once.
        dda_join_moves(prev_dda, dda);
        dda->n = dda->start_steps;
        if (dda->n == 0)
          dda->c = pgm_read_dword(&c0_P[dda->fast_axis]);
        else
          dda->c = (pgm_read_dword(&c0_P[dda->fast_axis]) *
                    int_inv_sqrt(dda->n)) >> 13;
        if (dda->c < dda->c_min)
          dda->c = dda->c_min;
      #else
        dda->n = 0;
        dda->c = pgm_read_dword(&c0_P[dda->fast_axis]);
      #endif

    } /* ! dda->total_steps == 0 */

    if (DEBUG_DDA && (debug_flags & DEBUG_DDA))
        serial_writestr_P(PSTR("] }\n"));

    // now prepare speed values for dc motor
    if (dda->waitfor)
    {
		// calculate dc motor speed according to distance, is linear interpolation ok here?
		int16_t dc_motor_speed = MIN_MOTOR_SPEED + ((margin_max_speed - MIN_MOTOR_SPEED) * ((int32_t)(distance - MAX_JUMP_LENGTH * 1000) / (1 - MAX_JUMP_LENGTH))) / 1000;
		if(dc_motor_speed > margin_max_speed) dc_motor_speed = margin_max_speed;
		else if(dc_motor_speed < MIN_MOTOR_SPEED) dc_motor_speed = MIN_MOTOR_SPEED;
		
		if(dc_motor_speed > 0)
		{
			if(prev_dda)
			{
				// start spinning motor slowly
				if(prev_dda->dc_motor_speed == 0)
					dc_motor_speed = MIN_MOTOR_SPEED;
				// speed up, decrease current dda speed if needed
				else if(dc_motor_speed - prev_dda->dc_motor_speed > JUMP_MOTOR_SPEED_DIFF_MAX )
					dc_motor_speed = prev_dda->dc_motor_speed + JUMP_MOTOR_SPEED_DIFF_MAX;
				// slow down, decrease previous dda speed if needed
				else if(prev_dda->dc_motor_speed - dc_motor_speed > JUMP_MOTOR_SPEED_DIFF_MAX_SLOWDOWN )
					queue_set_prev_dc_motor(dc_motor_speed);
			}
			else // no previous movement, start slowly as well
				dc_motor_speed = MIN_MOTOR_SPEED;
		}
		dda->dc_motor_speed = dc_motor_speed;
	}
	// other kind of movement requires motor to be stopped
	else
	{
		dda->dc_motor_speed = 0;
		queue_set_prev_dc_motor(MIN_MOTOR_SPEED);
	}
	// end of motor speed planner
    
    // next dda starts where we finish
    memcpy(&startpoint, &dda->endpoint, sizeof(TARGET));
    prev_dda = dda;
}


void dda_start(DDA *dda) {
    // called from interrupt context: keep it simple!

        // apply dc motor speed
        uint8_t queue_elements = queue_current_size();
        if(dda->dc_motor_speed == 0 )
        { 
			// turn off motor now for jump move
			if (dda->waitfor == 0)
				desired_speed = 0;
			else
				stop_dc_motor();
		}
		else
		{
			// slow down on almost empty buffer
			if(queue_elements < 4)
			{
				desired_speed = dda->dc_motor_speed / 2;
				if(desired_speed < MIN_MOTOR_SPEED)
					desired_speed = MIN_MOTOR_SPEED;
			}
			else // just use planned speed value from dda
				desired_speed = dda->dc_motor_speed;
		}
			
        if (dda->endstop_check)
            endstops_on();
        #ifdef TRIGGERED_MOVEMENT
        // if stepper movement dda is not allowed yet
        else if(dda->waitfor)
            return;
        #endif
        
        // buffer is empty, this is probably last move
        if(queue_elements == 0) 
            desired_speed = 0;

        
  if (DEBUG_DDA && (debug_flags & DEBUG_DDA))
    #ifdef STEPS_PER_M_Z
        sersendf_P(PSTR("Start: X %lq  Y %lq  Z %lq  F %lu\n"),
               dda->endpoint.axis[X], dda->endpoint.axis[Y],
               dda->endpoint.axis[Z], dda->endpoint.F);        
    #else
        sersendf_P(PSTR("Start: X %lq  Y %lq  F %lu\n"),
               dda->endpoint.axis[X], dda->endpoint.axis[Y],dda->endpoint.F);        
    #endif
    
        // get ready to go
        //psu_timeout = 0;
        
        // set direction outputs
        x_direction(dda->x_direction);
        y_direction(dda->y_direction);
        #ifdef STEPS_PER_M_Z
        z_direction(dda->z_direction);
        #endif
        #ifdef STEPS_PER_M_E
        e_direction(dda->e_direction);
        #endif
        
        // initialise state variable
        move_state.counter[X] = move_state.counter[Y] =  
        #ifdef STEPS_PER_M_Z
        move_state.counter[Z] = 
        #endif
        #ifdef STEPS_PER_M_E
        move_state.counter[E] = 
        #endif
        -(dda->total_steps >> 1);
        memcpy(&move_state.steps[X], &dda->delta[X], sizeof(uint32_t) * 4);
        move_state.endstop_stop = 0;
        
        move_state.step_no = 0;
        
        // ensure this dda starts
        dda->live = 1;
        
        // set timeout for first step
        timer_set(dda->c, 0);
    // else just a speed change, keep dda->live = 0

    current_position.F = dda->endpoint.F;
}

void dda_step(DDA *dda) {

  if (move_state.steps[X]) {
    move_state.counter[X] -= dda->delta[X];
    if (move_state.counter[X] < 0) {
            x_step();
      move_state.steps[X]--;
      move_state.counter[X] += dda->total_steps;
        }
    }
  if (move_state.steps[Y]) {
    move_state.counter[Y] -= dda->delta[Y];
    if (move_state.counter[Y] < 0) {
            y_step();
      move_state.steps[Y]--;
      move_state.counter[Y] += dda->total_steps;
        }
    }

#ifdef STEPS_PER_M_Z
if (move_state.steps[Z]) {
    move_state.counter[Z] -= dda->delta[Z];
    if (move_state.counter[Z] < 0) {
            x_step();
      move_state.steps[Z]--;
      move_state.counter[Z] += dda->total_steps;
        }
    }
#endif
#ifdef STEPS_PER_M_E
if (move_state.steps[E]) {
    move_state.counter[E] -= dda->delta[E];
    if (move_state.counter[E] < 0) {
            x_step();
      move_state.steps[E]--;
      move_state.counter[E] += dda->total_steps;
        }
    }
#endif
        move_state.step_no++;



  // If there are no steps left or an endstop stop happened, we have finished.
  //
  // TODO: with ACCELERATION_TEMPORAL this duplicates some code. See where
  //       dda->live is zero'd, about 10 lines above.
  if ((move_state.steps[X] == 0 && move_state.steps[Y] == 0 
    #ifdef STEPS_PER_M_Z 
        && move_state.steps[Z] == 0 
    #endif
    #ifdef STEPS_PER_M_E
        && move_state.steps[E] == 0
    #endif
      )  || (move_state.endstop_stop && dda->n <= 0)) {
        dda->live = 0;
        dda->done = 1;
        #ifdef LOOKAHEAD
        // If look-ahead was using this move, it could have missed our activation:
        // make sure the ids do not match.
        dda->id--;
        #endif
        // No need to restart timer here.
        // After having finished, dda_start() will do it.
    }
  else {
        //psu_timeout = 0;
      timer_set(dda->c, 0);
  }

    // turn off step outputs, hopefully they've been on long enough by now to register with the drivers
    // if not, too bad. or insert a (very!) small delay here, or fire up a spare timer or something.
    // we also hope that we don't step before the drivers register the low- limit maximum speed if you think this is a problem.
    unstep();
}

void dda_clock() {
  DDA *dda;
  static DDA *last_dda = NULL;
  uint8_t endstop_trigger = 0;

  uint32_t move_step_no, move_c;
  int32_t move_n;
  uint8_t recalc_speed;
  uint8_t current_id ;


  dda = queue_current_movement();
  if (dda != last_dda) {
    move_state.debounce_count_x =
    move_state.debounce_count_y =
    #ifdef STEPS_PER_M_Z
    move_state.debounce_count_z =
    #endif
    0;
    last_dda = dda;
  }

  if (dda == NULL)
    return;

  // Caution: we mangle step counters here without locking interrupts. This
  //          means, we trust dda isn't changed behind our back, which could
  //          in principle (but rarely) happen if endstops are checked not as
  //          endstop search, but as part of normal operations.
  if (dda->endstop_check && ! move_state.endstop_stop) {
    #ifdef X_MIN_PIN
    if (dda->endstop_check & 0x01) {
      if (x_min() == dda->endstop_stop_cond)
        move_state.debounce_count_x++;
      else
        move_state.debounce_count_x = 0;
      endstop_trigger = move_state.debounce_count_x >= ENDSTOP_STEPS;
    }
    #endif
    #ifdef X_MAX_PIN
    if (dda->endstop_check & 0x02) {
      if (x_max() == dda->endstop_stop_cond)
        move_state.debounce_count_x++;
      else
        move_state.debounce_count_x = 0;
      endstop_trigger = move_state.debounce_count_x >= ENDSTOP_STEPS;
    }
    #endif

    #ifdef Y_MIN_PIN
    if (dda->endstop_check & 0x04) {
      if (y_min() == dda->endstop_stop_cond)
        move_state.debounce_count_y++;
      else
        move_state.debounce_count_y = 0;
      endstop_trigger = move_state.debounce_count_y >= ENDSTOP_STEPS;
    }
    #endif
    #ifdef Y_MAX_PIN
    if (dda->endstop_check & 0x08) {
      if (y_max() == dda->endstop_stop_cond)
        move_state.debounce_count_y++;
      else
        move_state.debounce_count_y = 0;
      endstop_trigger = move_state.debounce_count_y >= ENDSTOP_STEPS;
    }
    #endif
   

    // If an endstop is definitely triggered, stop the movement.
    if (endstop_trigger) {
      #ifdef MILD_HOMING
        // For always smooth operations, don't halt abruptly,
        // but start deceleration here.
        ATOMIC_START
          move_state.endstop_stop = 1;
          move_step_no = dda->total_steps - move_state.steps[dda->fast_axis];

          if (move_step_no > dda->rampup_steps) {  // cruising?
            move_step_no = dda->total_steps - dda->rampdown_steps;
          }

          dda->rampdown_steps = move_step_no;
          dda->total_steps = move_step_no * 2;
          move_state.steps[dda->fast_axis] = move_step_no;
        ATOMIC_END

        // Not atomic, because not used in dda_step().
        dda->rampup_steps = 0; // in case we're still accelerating
      #else
        dda->live = 0;
      #endif
        
      endstops_off();
    }
  } /* ! move_state.endstop_stop */


    // For maths about stepper speed profiles, see
    // http://www.embedded.com/design/mcus-processors-and-socs/4006438/Generate-stepper-motor-speed-profiles-in-real-time
    // and http://www.atmel.com/images/doc8017.pdf (Atmel app note AVR446)
    ATOMIC_START
      current_id = dda->id;
      move_step_no = move_state.step_no;
      // All other variables are read-only or unused in dda_step(),
      // so no need for atomic operations.
    ATOMIC_END

    recalc_speed = 0;
    if (move_step_no < dda->rampup_steps) {
      #ifdef LOOKAHEAD
        move_n = dda->start_steps + move_step_no;
      #else
        move_n = move_step_no;
      #endif
      recalc_speed = 1;
    }
    else if (move_step_no >= dda->rampdown_steps) {
      #ifdef LOOKAHEAD
        move_n = dda->total_steps - move_step_no + dda->end_steps;
      #else
        move_n = dda->total_steps - move_step_no;
      #endif
      recalc_speed = 1;
    }
    if (recalc_speed) {
      if (move_n == 0)
        move_c = pgm_read_dword(&c0_P[dda->fast_axis]);
      else
        // Explicit formula: c0 * (sqrt(n + 1) - sqrt(n)),
        // approximation here: c0 * (1 / (2 * sqrt(n))).
        // This >> 13 looks odd, but is verified with the explicit formula.
        move_c = (pgm_read_dword(&c0_P[dda->fast_axis]) *
                  int_inv_sqrt(move_n)) >> 13;

      // TODO: most likely this whole check is obsolete. It was left as a
      //       safety margin, only. Rampup steps calculation should be accurate
      //       now and give the requested target speed within a few percent.
      if (move_c < dda->c_min) {
        // We hit max speed not always exactly.
        move_c = dda->c_min;
        
        // This is a hack which deals with movements with an unknown number of
        // acceleration steps. dda_create() sets a very high number, then,
        // but we don't want to re-calculate all the time.
        // This hack doesn't work with lookahead.
        #ifndef LOOKAHEAD
          dda->rampup_steps = move_step_no;
          dda->rampdown_steps = dda->total_steps - dda->rampup_steps;
        #endif
      }

      // Write results.
      ATOMIC_START
        /**
          Apply new n & c values only if dda didn't change underneath us. It
          is possible for dda to be modified since fetching values in the
          ATOMIC above, e.g. when a new dda becomes live.

          In case such a change happened, values in the new dda are more
          recent than our calculation here, anyways.
        */
      
        if (current_id == dda->id) {
          dda->c = move_c;
          dda->n = move_n;
        }
      ATOMIC_END
    }
    else {
      ATOMIC_START
        if (current_id == dda->id)
          // This happens only when !recalc_speed, meaning we are cruising, not
          // accelerating or decelerating. So it pegs our dda->c at c_min if it
          // never made it as far as c_min. 
          dda->c = dda->c_min;
      ATOMIC_END
    }
} 

/// update global current_position struct
void update_current_position() {
    DDA *dda = mb_tail_dda;
  uint8_t i;

  // Use smaller values to adjust to avoid overflow in later calculations,
  // (STEPS_PER_M_X / 1000) is a bit inaccurate for low STEPS_PER_M numbers.
  static const axes_uint32_t PROGMEM steps_per_mm_P = {
    ((STEPS_PER_M_X + 500) / 1000),
    ((STEPS_PER_M_Y + 500) / 1000)
    #ifdef STEPS_PER_M_Z
    ,((STEPS_PER_M_Z + 500) / 1000)
    #endif
    #ifdef STEPS_PER_M_E
    ,((STEPS_PER_M_E + 500) / 1000)
    #endif
  };

    if (queue_empty()) {
    for (i = X; i < AXIS_COUNT; i++) {
      current_position.axis[i] = startpoint.axis[i];
    }
    }
    else if (dda->live) {
    for (i = X; i < AXIS_COUNT; i++) {
      current_position.axis[i] = dda->endpoint.axis[i] -
          (int32_t)get_direction(dda, i) *
          // Should be: move_state.steps[i] * 1000000 / steps_per_m_P[i])
          // but steps[i] can be like 1000000 already, so we'd overflow.
          // Unfortunately, using muldiv() overwhelms the compiler.
          // Also keep the parens around this term, else results go wrong.
          ((move_state.steps[i] * 1000) / pgm_read_dword(&steps_per_mm_P[i]));
    }
#ifdef STEPS_PER_M_E
    if (dda->endpoint.e_relative)
      current_position.axis[E] =
          (move_state.steps[E] * 1000) / pgm_read_dword(&steps_per_mm_P[E]);
#endif
        // current_position.F is updated in dda_start()
    }
}
