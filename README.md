# Teathimble 
Teathimble is a minimalistics firmware to control varity of stepper motor based machines. It runs on most AVR (so arduino as well) microcontrollers and fits into ATmega16 MCU, providing great base for many projects. Teathimble is a trimmed fork of [Teacup](https://github.com/Traumflug/Teacup_Firmware) - lean and efficient firmware for RepRap printers. 

## Features
- Minimalistic code.
- Communication via buffered serial and text commands (G-code).
- True acceleration, interrupt based, with look-ahead and jerk calculation planning.
- Integer only math to save cpu cycles for performance boost.
- Up to 4 axis in straight or coreXY kinematics scheme.
- Decent performance: can run up to 48'000 evenly spaced steps/second on 20 MHz as mentioned by core developers.

## Work progress
Current code is checked on custom made arduino-like board top on ATmega32 to control automated [coil winding machine](https://gitlab.com/markol/Coil_winder). It is just a matter of formality to port configurations on other avaiable boards.
To run inside [simulator](https://reprap.org/wiki/SimulAVR) build project with enclosed makefile, you might also need [this arduino core](https://github.com/MCUdude/MightyCore) to turn custom board into arduino compatible.

## Source files description
Code is written in pure C, but source files are *.cpp* too keep it simple for arduino IDE.
| Name   |      Description       |
|----------|:-------------:|
| config.h |  Configuration definitions. |
|debug.cpp |  Debugging aids. |
|gcode_parser.cpp| G-code interpreter, instructions and their actions definitions. |
|kinematics.cpp|  Spearated code for different movement schemes. |
|maths.cpp| Calculation stuff, functions, constants, tables. |
|motion_planner.cpp| Ramping acceleration and lookahead related stuff. |
|motor.cpp| A rather complex block of math that figures out when to step each axis according to speed and acceleration profiles and received moves. |
|msg.cpp| For numbers parsing.  |
|pinio.cpp|  Initialise all I/O. |
|queue.cpp| The queue of moves received from the host. |
|serial.cpp| Serial buffers and communication management. |
|teathimble.ino.cpp| Code starts here. |
|teathimble.ino| Same as above, allows firmware to be built in Arduino IDE. |
|timer-avr.cpp| Timer management, used primarily by motor.c for timing steps. |
