# Teathimble 
Teathimble is a minimalistic firmware intended to control variety of stepper motor based machines. It runs on most AVR (so arduino as well) microcontrollers and fits into ATmega16 MCU, providing great base for many projects. Teathimble is a trimmed fork of [Teacup](https://github.com/Traumflug/Teacup_Firmware) - lean and efficient firmware for RepRap printers by Triffid Hunter, Traumflug, jakepoz, many others.

## Features
- Minimalistic code.
- Communication via buffered serial and text commands (G-code).
- True acceleration, interrupt based, with look-ahead and jerk calculation planning.
- Integer only math to save cpu cycles for performance boost.
- Up to 4 axis in straight or coreXY kinematics scheme.
- Decent performance: can run up to 48'000 evenly spaced steps/second on 20 MHz as mentioned by core developers.

## Work progress
For coli winder machine check this [branch](https://gitlab.com/markol/Coil_winder). Current code is checked on custom made arduino-like board top on ATmega32 to control winder. It is just a matter of formality to port configurations on other avaiable boards. You might also need [this arduino core](https://github.com/MCUdude/MightyCore) to turn custom board into arduino compatible.
To run inside simple [simulator](https://reprap.org/wiki/SimulAVR) build project with enclosed makefile, this might be come in handy for development.

## Building
Most of the configuration is described and is in *config.h* file.
To disable simulavr info sections, launch make with variable set `SIMULFLAGS= ` 
Example command for flashing Arduino UNO: `make flash MCU=atmega328p SIMULFLAGS= UPLOAD_PORT=/dev/ttyACM0`, assuming that configuration is complete. Makefile contains paths configuration to arduino core or mighty core, they need to be set up correctly. Project does not use arduino libraries at all, it is just compatible with Arduio IDE which should be capable of whole compilation and memory programming task, just open the *.ino* file.

## Source files description
Code is written in pure C.

|      Name      |                              Description                                                                                               |
|----------------|:--------------------------------------------------------------------------------------------------------------------------------------:|
| config.h       |  Configuration constants definitions.                                                                                                  |
|debug.c         |  Debugging aids.                                                                                                                       |
|gcode_parser.c  | G-code interpreter, instructions and their actions definitions.                                                                        |
|kinematics.c    |  Separated code for different movement schemes.                                                                                        |
|maths.c         | Calculation stuff, functions, constants, tables.                                                                                       |
|motion_planner.c| Ramping acceleration and lookahead related stuff.                                                                                      |
|motor.c         | A rather complex block of math that figures out when to step each axis according to speed and acceleration profiles and received moves.|
|msg.c           | For numbers parsing.                                                                                                                   |
|pinio.c         |  Initialize all I/O.                                                                                                                   |
|queue.c         | The queue of moves received from the host.                                                                                             |
|serial.c        | Serial buffers and communication management.                                                                                           |
|teathimble.ino.c| Code starts here.                                                                                                                      |
|teathimble.ino  | Same as above, allows firmware to be built in Arduino IDE.                                                                             |
|timer-avr.c     | Timer management, used primarily by motor.c for timing steps.                                                                          |