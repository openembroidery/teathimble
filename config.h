#ifndef _CONFIG_H
#define _CONFIG_H

#include <Arduino.h>
#include "arduino_32U4.h"

#define KINEMATICS_STRAIGHT

/** \def X_MIN X_MAX Y_MIN Y_MAX Z_MIN Z_MAX
  Soft axis limits. Define them to your machine's size relative to what your
  G-code considers to be the origin (typically the bed's center or the bed's
  front left corner).

  Note that relocating the origin at runtime with G92 will also relocate these
  limits.

  Not defining them at all will disable limits checking and make the binary
  about 250 bytes smaller. Enabling only some of them is perfectly fine.

    Units: millimeters
    Sane values: according to printer build room size
    Valid range: -1000.0 to 1000.0
*/
/*
#define X_MIN                    0.0
#define X_MAX                    200.0

#define Y_MIN                    0.0
#define Y_MAX                    200.0

#define Z_MIN                    0.0
#define Z_MAX                    140.0
*/
/*steps per meter*/

#define STEPS_PER_M_X            1280000
#define STEPS_PER_M_Y            1280000
#define STEPS_PER_M_Z            1280000
#define STEPS_PER_M_E            96271

/** \def SEARCH_FEEDRATE_X SEARCH_FEEDRATE_Y SEARCH_FEEDRATE_Z
  Used when doing precision endstop search and as default feedrate. 
  (mm / min)  60 mm / min = 1 mm/sec
*/
#define SEARCH_FEEDRATE_X        400
#define SEARCH_FEEDRATE_Y        400
#define SEARCH_FEEDRATE_Z        400

/** \def MAXIMUM_FEEDRATE_X MAXIMUM_FEEDRATE_Y MAXIMUM_FEEDRATE_Z MAXIMUM_FEEDRATE_E
  Used for G0 rapid moves and as a cap for all other feedrates. (mm / min) 
*/
#define MAXIMUM_FEEDRATE_X       6000
#define MAXIMUM_FEEDRATE_Y       6000
#define MAXIMUM_FEEDRATE_Z       6000
#define MAXIMUM_FEEDRATE_E       20000

/** \def ACCELERATION
  How fast to accelerate when using ACCELERATION_RAMPING. Start with 10 for
  milling (high precision) or 1000 for printing.

    Units: mm/s^2
    Useful range: 1 to 10'000
*/
#define ACCELERATION             100

/** \def LOOKAHEAD
  Define this to enable look-ahead during *ramping* acceleration to smoothly
  transition between moves instead of performing a dead stop every move.
  Enabling look-ahead requires about 3600 bytes of flash memory.
*/
#define LOOKAHEAD

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

  Having these values too low results in more than neccessary slowdown at
  movement crossings, but is otherwise harmless. Too high values can result
  in stepper motors suddenly stalling. If angles between movements in your
  G-code are small and your printer runs through entire curves full speed,
  there's no point in raising the values.

    Units: mm/min
    Sane values: 0 to 400
    Valid range: 0 to 65535
*/
#define MAX_JERK_X               80
#define MAX_JERK_Y               80
#define MAX_JERK_Z               0
#define MAX_JERK_E               200


/** \def BAUD
  Baud rate for the serial RS232 protocol connection to the host. Usually
  115200, other common values are 19200, 38400 or 57600. Ignored when USB_SERIAL
  is defined.
*/
#define BAUD                     9600

/** \def XONXOFF
  Xon/Xoff flow control.

  Redundant when using RepRap Host for sending G-code, but mandatory when
  sending G-code files with a plain terminal emulator, like GtkTerm (Linux),
  CoolTerm (Mac) or HyperTerminal (Windows).
*/
#define XONXOFF

/**************PINOUT****************/

#define X_STEP_PIN               PC3 // 19 PC3
#define X_DIR_PIN                PC2 // 18 PC2
#define X_MIN_PIN                DIO3
#define X_MAX_PIN                DIO2
#define X_ENABLE_PIN             PC4 // 20 PC4
#define X_INVERT_DIR
#define X_INVERT_MIN
#define X_INVERT_MAX
#define X_INVERT_ENABLE

#define Y_STEP_PIN               DIO9 //22 PC6
#define Y_DIR_PIN                PC5 //21 PC5
#define Y_MIN_PIN                DIO8
#define Y_MAX_PIN                DIO7
#define Y_ENABLE_PIN             DIO10 //23 PC7
#define Y_INVERT_DIR
#define Y_INVERT_MIN
#define Y_INVERT_MAX
#define Y_INVERT_ENABLE

#define Z_STEP_PIN               DIO11
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

#define ENCODER_PIN 17


#define PWR_OUT1_PIN 18
#define PWR_OUT2_PIN 19
#define ANALOG_IN_PIN 33
#define SWITCH1_PIN 34
#define SWITCH2_PIN 35
#define SWITCH3_PIN 16

/** \def MOVEBUFFER_SIZE
  Move buffer size, in number of moves.

  Note that each move takes a fair chunk of ram (107 bytes as of this writing),
  so don't make the buffer too big. However, a larger movebuffer will probably
  help with lots of short consecutive moves, as each move takes a bunch of
  math (hence time) to set up so a longer buffer allows more of the math to
  be done during preceding longer moves.
*/
#define MOVEBUFFER_SIZE          8

/** \def F_CPU
  Actual CPU clock rate. #ifndef required for Arduino compatibility.
*/
#ifndef F_CPU
#define F_CPU                    16000000UL
#endif

/** \def MOTHERBOARD
  This is the motherboard, as opposed to the extruder. See extruder/ directory
  for GEN3 extruder firmware.
*/
#define MOTHERBOARD

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
#define SIMINFO
#define DEBUG

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
    #define     DEBUG_DDA           1
    #define     DEBUG_POSITION      1
    #define     DEBUG_ECHO          1
  #define DEBUG_INFO                1
  #define DEBUG_DRYRUN              1
#endif
extern volatile uint8_t debug_flags;

#ifndef BSS
  #define BSS __attribute__ ((__section__ (".bss")))
#endif

#endif  /* _CONFIG_H */