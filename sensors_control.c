#include "sensors_control.h"

uint8_t last_direction = 0;
uint8_t stop_motor_flag = 0;
uint8_t interrupt_ticks = 0;
uint16_t motor_pulses = 0;

int32_t ki, kp;
int16_t error_speed_sum;
int16_t desired_speed, error_speed;
int16_t margin_max_speed = DEFAULT_MOTOR_SPEED;

/// Init INT0 and INT1 interrupts for optical sensors, init pwm and timers for dc motor speed controller
void sensing_init()
{
	PULL_OFF(PD2);
    PULL_OFF(ENCODER_PIN_A);
    PULL_OFF(ENCODER_PIN_B);
    SET_INPUT(PD2);
    SET_INPUT(ENCODER_PIN_A);
    SET_INPUT(ENCODER_PIN_B);

  #if defined (__AVR_ATmega32U4__) || defined (__AVR_ATmega32__) || \
            defined (__AVR_ATmega16__)
	// externally trigged interrupts setup
    MCUCR |= MASK(ISC00) | MASK(ISC11) | MASK(ISC10); /* INT0 - any change, INT1 - raising edge */
    GICR |= MASK(INT0) | MASK(INT1);        /* enable INT0 and INT1 */

    // setup PWM timers: fast PWM
	// Warning: these are not consistent across all AVRs
	TCCR0 = MASK(WGM01) | MASK(WGM00) | MASK(COM01);
	// pwm frequency
	#ifndef FAST_PWM
		TCCR0 |= MASK(CS02); // F_CPU / 256
	#else
		TCCR0 |=  MASK(CS01); // F_CPU / 8
	#endif

	#define MOTOR_PWM OCR0
	OCR0 = 0;
	PB4_DDR = 0xFF;

	// timer 1 is used for stepping

    TCCR2 = MASK(CS20) | MASK(CS21) | MASK(CS22); // F_CPU / 256 / 1024
    OCR2 = 0;
    // enable timer overflow int
    TIMSK |= MASK(TOIE2);
  #else
    // externally trigged interrupts setup
    EICRA |= MASK(ISC00) | MASK(ISC11) | MASK(ISC10); /* INT0 - any logic change INT1 - raising edge */
    EIMSK |= MASK(INT0) | MASK(INT1);        /* enable INT0 and INT1 */

	// timer 1 is used for stepping

    // PWM frequencies in TCCR2B, see page 156 of the ATmega644 reference.
    TCCR2A = 0;
    TCCR2B = MASK(CS20) | MASK(CS21) | MASK(CS22); // F_CPU / 256 / 1024

    OCR2A = 0;
    OCR2B = 0;

    // pwm frequency
    #ifndef FAST_PWM
		TCCR0B =  (TCCR0B & 0b11111000) | 0x04;
	#else
		TCCR0B =  (TCCR0B & 0b11111000) | 0x02;
	#endif
		// init of timer 0 for pwm motor control. ouput pin is PD6
		TCCR0A = MASK(WGM01) | MASK(WGM00) | MASK(COM0A1);
		#define MOTOR_PWM OCR0A
		OCR0A = 0;
		OCR0B = 0;
		SET_OUTPUT(PD6);

    // enable timer overflow int
    TIMSK2 = MASK(TOIE2);
  #endif

    error_speed = desired_speed = error_speed_sum = 0;
	kp = KP_FACTOR;
	ki = KI_FACTOR;

}
void set_dc_motor_speed(int16_t value)
{
	desired_speed = value;
}

/// sets machine maximal work speed
void set_dc_motor_speed_margin(int16_t value)
{
	if (value <= MAX_MOTOR_SPEED)
	{
		if (value >= MIN_MOTOR_SPEED)
			margin_max_speed = value;
		else
			margin_max_speed = MIN_MOTOR_SPEED;
	}
	else
		margin_max_speed = MAX_MOTOR_SPEED;
}

/// activates stop of the the motor on needle downwards interrupt
/// mode 0 - stop on first interrupt, any other value - stop on next one interrupt
void stop_dc_motor(uint8_t mode)
{
    if (stop_motor_flag || desired_speed == 0) return;
    if (mode)
    {
        desired_speed = MIN_MOTOR_SPEED;
        stop_motor_flag = 2;
    }
    else
    {
        desired_speed = MIN_MOTOR_SPEED/2;
        stop_motor_flag = 1;
    }
}

/// PI control update procedure
void update_dc_motor()
{
	int16_t pwm_pulse;
	if (desired_speed > 3 )
	{
		if(interrupt_ticks <= 0) return;
		// ticking interrupt is called at about 61 Hz rate
		uint8_t time_interval = (F_CPU / 256 / 1024 / interrupt_ticks);
		//calculate motor speed, unit is decimal rpm
		int16_t pv_speed = ( 60 * (motor_pulses*POINT_SHIFT/PULSES_PER_TURN) * time_interval ) / POINT_SHIFT;

		interrupt_ticks = 0;
		motor_pulses = 0;
		error_speed = desired_speed - pv_speed;
		pwm_pulse = (int32_t)(((int32_t) error_speed*kp) + ((int32_t) error_speed_sum*ki)) / POINT_SHIFT;
		error_speed_sum += error_speed; //sum of error
		// integral limitation
		if (error_speed_sum >ACCUMULATOR_LIMIT) error_speed_sum = ACCUMULATOR_LIMIT;
		else if (error_speed_sum < -ACCUMULATOR_LIMIT) error_speed_sum = -ACCUMULATOR_LIMIT;
   }
   else
   {
		error_speed = 0;
		error_speed_sum = 0;
		pwm_pulse = 0;
		interrupt_ticks = 0;
   }
	// set new motor power
	if (pwm_pulse < 255 && pwm_pulse > 0)
	{
		MOTOR_PWM = pwm_pulse;
	}
   else
   {
		if (pwm_pulse>255)
		{
			MOTOR_PWM = 255;
		}
		else
		{
			MOTOR_PWM = 0;
		}
   }
}

/// needle interrupt
ISR(INT1_vect)
{
	// check if second sensor is trigged to estimate needle direction
	if(direction_encoder())
	{
		// false alarm, last interrupt was about the same direction
		if(last_direction)
			return;
		last_direction = 1;
        if(stop_motor_flag == 2)
        {
            --stop_motor_flag;
            desired_speed = MIN_MOTOR_SPEED/2;
        }
		if(mb_tail_dda && !mb_tail_dda->live && !stop_motor_flag)
		{
			mb_tail_dda->waitfor = 0;
			dda_start(mb_tail_dda);
			sei();
		}
        //serial_writestr_P(PSTR("int1 up\n"));
	}
	else // needle goes downwards
	{
		if(!last_direction)
			return;
		last_direction = 0;
		//TODO: stop any movement currently in progress
		if(stop_motor_flag == 1)
		{
			desired_speed = 0;
			stop_motor_flag = 0;
            if(mb_tail_dda && !mb_tail_dda->live)
            {
                mb_tail_dda->waitfor = 0;
                dda_start(mb_tail_dda);
                sei();
            }
		}
        //serial_writestr_P(PSTR("int1 down\n"));
	}
}

/// motor rotary encoder interrupt for speed measurement
ISR(INT0_vect)
{
	++motor_pulses;
}

/// timer tick interrupt
ISR(TIMER2_OVF_vect)
{
	++interrupt_ticks;
}
