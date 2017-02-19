OBJECTS=main.o error.o init.o lexer.o parser.o symbol.o 


elf:
	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -M -MG -MP -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 motor_control.ino

	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -M -MG -MP -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 motor_control.ino

	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -M -MG -MP -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 motor_control.ino

	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -M -MG -MP -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 motor_control.ino

	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -M -MG -MP -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 debug.cpp

	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -M -MG -MP -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 gcode_parser.cpp

	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -M -MG -MP -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 kinematics.cpp

	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -M -MG -MP -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 maths.cpp

	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -M -MG -MP -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 motor.cpp

	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -M -MG -MP -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 motor_control.ino

	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -M -MG -MP -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 msg.cpp

	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -M -MG -MP -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 pinio.cpp

	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -M -MG -MP -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 queue.cpp

	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -M -MG -MP -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 serial.cpp

	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -M -MG -MP -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 timer-avr.cpp

	avr-g++ -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics  -w -x c++ -E -CC -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 motor_control.ino
	
	avr-g++ -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 debug.cpp -o debug.cpp.o
	
	avr-g++ -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 gcode_parser.cpp -o gcode_parser.cpp.o

	avr-g++ -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 kinematics.cpp -o kinematics.cpp.o

	avr-g++ -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 maths.cpp -o maths.cpp.o

	avr-g++ -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 motor.cpp -o motor.cpp.o

	avr-g++ -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 motor_control.ino -o motor_control.ino.o

	avr-g++ -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 msg.cpp -o msg.cpp.o

	avr-g++ -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 pinio.cpp -o pinio.cpp.o


	avr-g++ -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 queue.cpp -o queue.cpp.o
	
	avr-g++ -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 serial.cpp -o serial.cpp.o

	avr-g++ -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 timer-avr.cpp -o timer-avr.cpp.o

	avr-g++ -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 motor_control.ino.cpp -o motor_control.ino.cpp.o
	
	avr-g++ -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 motion_planner.cpp -o motion_planner.cpp.o
	
	avr-g++ -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 /usr/share/arduino/hardware/arduino/avr/cores/MightyCore/main.cpp -o main.cpp.o
	
	avr-g++ -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 /usr/share/arduino/hardware/arduino/avr/cores/MightyCore/wiring.c -o wiring.o
	
	avr-gcc -c -g -Os -Wall -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega32 -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_ATmega32 -DARDUINO_ARCH_AVR   -I/usr/share/arduino/hardware/arduino/avr/cores/MightyCore -I/usr/share/arduino/hardware/arduino/avr/variants/mega32 /usr/share/arduino/hardware/arduino/avr/cores/MightyCore/hooks.c -o hooks.o
	
	#avr-gcc -Wall -Os -Wl,--gc-sections -mmcu=atmega32  -o motor_control.ino.elf debug.cpp.o gcode_parser.cpp.o kinematics.cpp.o maths.cpp.o motor.cpp.o motor_control.ino.cpp.o msg.cpp.o pinio.cpp.o queue.cpp.o serial.cpp.o timer-avr.cpp.o motion_planner.cpp.o main.cpp.o wiring.o hooks.o -lm -Wl,--section-start=.siminfo=0x900000
	
	avr-gcc -Wall -Os -Wl,--section-start=.siminfo=0x900000 -mmcu=atmega32  -o motor_control.ino.elf debug.cpp.o gcode_parser.cpp.o kinematics.cpp.o maths.cpp.o motor.cpp.o motor_control.ino.cpp.o msg.cpp.o pinio.cpp.o queue.cpp.o serial.cpp.o timer-avr.cpp.o motion_planner.cpp.o main.cpp.o wiring.o hooks.o -lm 

hex: elf	
	avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  motor_control.ino.elf motor_control.ino.eep
	
	avr-objcopy -O ihex -R .eeprom  motor_control.ino.elf motor_control.ino.hex
