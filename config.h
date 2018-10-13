#ifndef _CONFIG_H
#define _CONFIG_H

#include <Arduino.h>
#include "arduino.h"

/** \def KINEMATICS_STRAIGHT KINEMATICS_COREXY

  This defines the type of kinematics your device uses. That's essential!

  Valid values:

  KINEMATICS_STRAIGHT
    Motors move axis directions directly. This is the
    traditional type, most popular. Set this for coil winding machine.

  KINEMATICS_COREXY
    A bot using CoreXY kinematics. Typical for CoreXY
    are long and crossing toothed belts and a carriage
    moving on the X-Y-plane.
*/
//#define KINEMATICS_STRAIGHT
#define KINEMATICS_COREXY


/** \def X_MIN X_MAX Y_MIN Y_MAX Z_MIN Z_MAX
  Soft axis limits. Define them to your machine's size relative to what your
  G-code considers to be the origin.

  Note that relocating the origin at runtime with G92 will also relocate these
  limits.

  Not defining them at all will disable limits checking and make the binary
  about 250 bytes smaller. Enabling only some of them is perfectly fine.

    Units: millimeters
    Sane values: according to device build room size
    Valid range: -1000.0 to 1000.0
*/

#define X_MIN                    0.0
#define X_MAX                    280.0

#define Y_MIN                    0.0
#define Y_MAX                    280.0

//#define Z_MIN                    0.0
//#define Z_MAX                    140.0


/** \def STEPS_PER_M_X STEPS_PER_M_Y STEPS_PER_M_Z STEPS_PER_M_E
  Steps per meter ( = steps per mm * 1000 ), calculate these values
  appropriate for your machine.

  All numbers are integers, so no decimal point, please :-)

    Valid range: 20 to 4'0960'000 (0.02 to 40960 steps/mm)
*/
/**
 * X = Y = (200 motor steps * 16 microsteps ) / (18 pulley tooth count * 2.03 mm tooth spacing ) * 1000 mm per meter
  **/
#define STEPS_PER_M_X            87575
#define STEPS_PER_M_Y            87575
/*#define STEPS_PER_M_Z            1280000
#define STEPS_PER_M_E            96271
*/
/** \def SEARCH_FEEDRATE_X SEARCH_FEEDRATE_Y SEARCH_FEEDRATE_Z
  Used when doing precision endstop search and as default feedrate. 
  (mm / min)  60 mm / min = 1 mm/sec
*/
#define SEARCH_FEEDRATE_X        10000
#define SEARCH_FEEDRATE_Y        10000
//#define SEARCH_FEEDRATE_Z        400

/** \def MAXIMUM_FEEDRATE_X MAXIMUM_FEEDRATE_Y MAXIMUM_FEEDRATE_Z MAXIMUM_FEEDRATE_E
  Used for G0 rapid moves and as a cap for all other feedrates. (mm / min) 
*/
#define MAXIMUM_FEEDRATE_X       33000
#define MAXIMUM_FEEDRATE_Y       33000
/*#define MAXIMUM_FEEDRATE_Z       6000
#define MAXIMUM_FEEDRATE_E       20000
*/
/** \def ACCELERATION
  How fast to accelerate when using ACCELERATION_RAMPING. Start with 10 for
  milling (high precision) or 1000 for printing.
  High values affect aproximation accuracy for low speeds.

    Units: mm/s^2
    Useful range: 1 to 10'000
*/
#define ACCELERATION             2000

/** \def ENDSTOP_CLEARANCE

  When hitting an endstop, Teacup properly decelerates instead of doing an
  aprupt stop to save your mechanics. Ineviteably, this means it overshoots
  the endstop trigger point by some distance.

  To deal with this, Teacup adapts homing movement speeds to what your
  endstops can deal with. The higher the allowed acceleration ( = deceleration,
  see #define ACCELERATION) and the more clearance the endstop comes with,
  the faster Teacup will do homing movements.

  Set here how many micrometers (mm * 1000) your endstop allows the carriage
  to overshoot the trigger point. Typically 1000 or 2000 for mechanical
  endstops, more for optical ones. You can set it to zero, in which case
  SEARCH_FEEDRATE_{XYZ} is used, but expect very slow homing movements.

    Units: micrometers
    Sane values: 0 to 20000   (0 to 20 mm)
    Valid range: 0 to 1000000
*/
#define ENDSTOP_CLEARANCE_X      100
#define ENDSTOP_CLEARANCE_Y      100

/** \def ENDSTOP_STEPS
  Number of steps to run into the endstops intentionally. As endstops trigger
  false alarm sometimes, Teacup debounces them by counting a number of
  consecutive positives.

  Use 4 or less for reliable endstops, 8 or even more for flaky ones.

    Valid range: 1...255.
*/
#define ENDSTOP_STEPS  2

/** \def MILD_HOMING
 Define this to prevent abrupt stop of movement when endstop is trigged. For lightweight mechanics 
 and low max speed or high acceleration values it is ok to keep this disabled.
*/
#define MILD_HOMING

/** \def TRIGGERED_MOVEMENT
 Define this to start new G1 movement only by external interrupt from additional sensor switch.
*/
#define TRIGGERED_MOVEMENT


/** \def LOOKAHEAD
  Define this to enable look-ahead during *ramping* acceleration to smoothly
  transition between moves instead of performing a dead stop every move.
  Enabling look-ahead requires about 3600 bytes of flash memory.
*/
//#define LOOKAHEAD

/** \def MAX_JERK_X MAX_JERK_Y MAX_JERK_Z MAX_JERK_E
  When performing look-ahead, we need to decide what an acceptable jerk to the
  mechanics is. Look-ahead attempts to instantly change direction at movement
  crossings, which means instant changes in the speed of the axes participating
  in the movement. Define here how big the speed bumps on each of the axes is
  allowed to be.

  If you want a full stop before and after moving a specific axis, define
  MAX_JERK of this axis to 0. This is often wanted for the Z axis. If you want
  to ignore jerk on an axis, define it to twice the maximum feedrate of this
  axis.

  Having these values too low results in more than necessary slowdown at
  movement crossings, but is otherwise harmless. Too high values can result
  in stepper motors suddenly stalling. If angles between movements in your
  G-code are small and your printer runs through entire curves full speed,
  there's no point in raising the values.

    Units: mm/min
    Sane values: 0 to 400
    Valid range: 0 to 65535
*/
#define MAX_JERK_X               400
#define MAX_JERK_Y               400
/*#define MAX_JERK_Z               0
#define MAX_JERK_E               200*/


/** \def BAUD
  Baud rate for the serial RS232 protocol connection to the host. Usually
  115200, other common values are 19200, 38400 or 57600. Ignored when USB_SERIAL
  is defined.
*/
#define BAUD                     115200

/** \def XONXOFF
  Xon/Xoff flow control.

  Redundant when using RepRap Host for sending G-code, but mandatory when
  sending G-code files with a plain terminal emulator, like GtkTerm (Linux),
  CoolTerm (Mac) or HyperTerminal (Windows).
*/
#define XONXOFF

/** \def MOTHERBOARD
 ***************MOTHERBOARD***************
 * Define this to use one of predefined configurations.
 **/
//#define MOTHERBOARD 2
#include "boards.h"


/** \def PINOUT
 ***************PINOUT***************
 * Here you can setup pin functions, depending on board configuration. 
 * Comment board define above to enable this customizable config.
 **/

#ifndef MOTHERBOARD
#define MOTHERBOARD
#define X_STEP_PIN               PC3
// 19 PC3
#define X_DIR_PIN                PC2
// 18 PC2
#define X_MIN_PIN                PA5
//35 PA5
//#define X_MAX_PIN                DIO2
#define X_ENABLE_PIN             PC4
// 20 PC4
#define X_INVERT_DIR
#define X_INVERT_MIN
#define X_INVERT_MAX
#define X_INVERT_ENABLE

#define Y_STEP_PIN               DIO9
//22 PC6
#define Y_DIR_PIN                PC5
//21 PC5
// 16 PD2
#define Y_MIN_PIN                PA6
//#define Y_MAX_PIN                DIO7
#define Y_ENABLE_PIN             DIO10
//23 PC7
#define Y_INVERT_DIR
#define Y_INVERT_MIN
#define Y_INVERT_MAX
#define Y_INVERT_ENABLE

/*#define Z_STEP_PIN               DIO11
#define Z_DIR_PIN                DIO12
#define Z_MIN_PIN                DIO13
#define Z_MAX_PIN                DIO14
#define Z_ENABLE_PIN             DIO15
#define Z_INVERT_DIR
#define Z_INVERT_MIN
#define Z_INVERT_MAX
#define Z_INVERT_ENABLE

#define E_STEP_PIN               DIO23
#define E_DIR_PIN                DIO0
#define E_ENABLE_PIN             DIO22
#define E_INVERT_DIR
#define E_INVERT_ENABLE
*/

/** \def ENCODER_PIN
 * ENCODER_PIN_A must be connected to INT1 pin for external interrupt to work!
 * Motor encoder needs INT0 pin.
 **/
#define ENCODER_PIN_A PD3
#define ENCODER_PIN_B PA4
#define INVERT_DIRECTION_ENCODER

/** \def PULSES_PER_TURN
 * Motor encoder pulses per one machine shaft revolution. 
 * If encoder wheel is mounted on motor pulley for better resolution,
 * its gear ratio must be taken into account. Every change of square wave from encoder
 * counts as one tick so number of pulses per wheel revolution equals to number of blades doubled.  
 **/
#define PULSES_PER_TURN 84

/** \def MOTOR_SPEED
 * Defines minimal and maximal functional speed of machine shaft in RPM
  
    Units: revolutions/min, stitches/min
    Sane values: 50 to 1900
    Valid range: 1 to 3000
 **/
#define MIN_MOTOR_SPEED 80
#define MAX_MOTOR_SPEED 400

/** \def KX_FACTOR
 * Proportional and integral factor for speed controller. 
 * True value of the factor equals to: value / POINT_SHIFT
  
    Sane values: 100 to 90000
    Valid range: 1 to 900000
 **/
#define KP_FACTOR 40000
#define KI_FACTOR 2000

/** \def ACCUMULATOR_LIMIT
 * Limits for integral part of the controller. Higher values mean that controller
 * remebers more errors in past and might make it unstable on varying load and speed changes. 
 * Don't exceed int 16 bit 32,768 value.
    
    Sane values: 100 to 9000
    Valid range: 0 to 32768
 **/
#define ACCUMULATOR_LIMIT 4000

/** \def POINT_SHIFT
 * PI controller uses fixed point arithmetic, change number of ZEROS for better or worse accuracy.
 * Default value is enough in most cases, bigger numbers might cause overflow.
 
    Sane values: 100 to 100000
    Valid range: 1 to 1000000
 **/
#define POINT_SHIFT 100000

#define PWR_OUT1_PIN PD4
#define PWR_OUT2_PIN PD5
#define ANALOG_IN_PIN 33
#define SWITCH1_PIN 34
#define SWITCH2_PIN 35
#define SWITCH3_PIN PD2

/** \def MAX_JUMP_LENGTH
 * This parameter tells motor speed planner to use lowest defined speed 
 * for stitches of length close to this value.
    
    Units: mm
    Sane values: 5 to 16
    Valid range: 1 to 200
 **/
#define MAX_JUMP_LENGTH 12

/** \def JUMP_MOTOR_SPEED_DIFF_MAX
 * This parameter tells motor speed planner to try keep maximal possible 
 * difference in speed between all jumps in range of this value.
    
    Units: rpm
    Sane values: 50 to 400
    Valid range: 1 to 2000
 **/
#define JUMP_MOTOR_SPEED_DIFF_MAX 100

#endif

/** \def MOVEBUFFER_SIZE
  Move buffer size, in number of moves.

  Note that each move takes a fair chunk of ram (107 bytes as of this writing),
  so don't make the buffer too big. However, a larger movebuffer will probably
  help with lots of short consecutive moves, as each move takes a bunch of
  math (hence time) to set up so a longer buffer allows more of the math to
  be done during preceding longer moves.
*/
#define MOVEBUFFER_SIZE          8

/** \def USE_INTERNAL_PULLUPS

  Most controller chips feature internal pullup resistors on their input pins,
  which get used for endstops by turning on this switch. Don't turn it on when
  using endstops which need no pull resistor, e.g. optical endstops, because
  pull resistors are counterproductive there.

  One can't use USE_INTERNAL_PULLUPS and USE_INTERNAL_PULLDOWNS at the same
  time, of course.
*/
#define USE_INTERNAL_PULLUPS

/** \def F_CPU
  Actual CPU clock rate. #ifndef required for Arduino compatibility.
*/
#ifndef F_CPU
#define F_CPU                    16000000UL
#endif

/** \def DEBUG_LED_PIN

  Enable flashing of a LED during motor stepping.

  Disabled by default. Uncommenting this makes the binary a few bytes larger
  and adds a few cycles to the step timing interrrupt in timer.c. Also used
  for precision profiling (profiling works even without actually having such
  a LED in hardware), see
  http://reprap.org/wiki/Teacup_Firmware#Doing_precision_profiling
*/
#define DEBUG_LED_PIN            PC1

////////////////DEBUG/////////////////////
//#define SIMINFO
//#define DEBUG 1

#ifdef  DEBUG
  #define DEBUG_ECHO       1
  #define DEBUG_INFO       2
  #define DEBUG_ERRORS     4
  #define DEBUG_DRYRUN     8
  #define DEBUG_PID       16
  #define DEBUG_DDA       32
  #define DEBUG_POSITION  64
#else
    // by setting these to zero, the compiler should optimise the relevant code out
    #define     DEBUG_PID           0
    #define     DEBUG_DDA           0
    #define     DEBUG_POSITION      0
    #define     DEBUG_ECHO          0
  #define DEBUG_INFO                0
  #define DEBUG_DRYRUN              0
#endif
extern volatile uint8_t debug_flags;

#ifndef BSS
  #define BSS __attribute__ ((__section__ (".bss")))
#endif


#endif  /* _CONFIG_H */
