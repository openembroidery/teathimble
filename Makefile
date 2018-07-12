# This makefile for now is used to build executable for simavr for debbuging.
# Click & forget arduino IDE should deal with mcu flashing.

# MCU ?= atmega168
# MCU ?= atmega328p
#MCU ?= atmega644
# MCU ?= atmega644
# MCU ?= atmega1284p
# MCU ?= atmega1280
# MCU ?= atmega2560
# MCU ?= at90usb1286
 MCU ?= atmega32

# CPU clock rate
# F_CPU ?= 8000000L
F_CPU ?= 16000000L
# F_CPU ?= 20000000L

ARDUINO_CORE = /usr/share/arduino/hardware/arduino/avr/cores/MightyCore
ARDUINO_VARIANT = /usr/share/arduino/hardware/arduino/avr/variants/mega32

#avr gcc
CC = avr-gcc 
#avr objeccopy
OBJCOPY = avr-objcopy

CFLAGS = -w -std=gnu++11 -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -DF_CPU=16000000L -I$(ARDUINO_CORE) -I$(ARDUINO_VARIANT)

CFLAGS += -mmcu=$(MCU)
SIMULFLAGS = -Wl,--section-start=.siminfo=0x900000

#debug?
CFLAGS += -g

OBJFLAG = -O ihex


EXECUTABLE = teathimble
SOURCES = ${wildcard *.cpp $(ARDUINO_CORE)/main.cpp $(ARDUINO_CORE)/wiring.c $(ARDUINO_CORE)/hooks.c}
HEADERS = ${wildcard *.h}
OBJECTS = ${SOURCES:.c=.o}

.PHONY: all
all: ${EXECUTABLE}

$(EXECUTABLE): $(OBJECTS) buildnumber.num
	$(CC) $(CFLAGS)  $(SIMULFLAGS) -o $(EXECUTABLE).elf $(OBJECTS) -lm
	$(OBJCOPY) $(OBJFLAG) -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $(EXECUTABLE).elf $(EXECUTABLE).eep

	$(OBJCOPY) $(OBJFLAG) -R .eeprom  $(EXECUTABLE).elf $(EXECUTABLE).hex
	@echo "-- Build: " $$(cat buildnumber.num)

depend: $(SOURCES)
	@echo "calling depend"
	$(CC) $(CFLAGS) -Os -c -MM $^ > $@

-include depend

# Buildnumber administratie
buildnumber.num: $(OBJECTS)
	@if ! test -f buildnumber.num; then echo 0 > buildnumber.num; fi
	@echo $$(($$(cat buildnumber.num)+1)) > buildnumber.num

# Create a clean environment
.PHONY: clean
clean:
	$(RM) $(EXECUTABLE)

# Clean up dependency file  
.PHONY: clean-depend
clean-depend: clean
	$(RM) depend   
