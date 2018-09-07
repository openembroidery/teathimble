# This simple makefile by default builds executable for simulavr for debbuging.
# To disable simulavr info sections, launch make with SIMULFLAGS= 
# Example command for flashing uno:
# make flash MCU=atmega328p SIMULFLAGS= UPLOAD_PORT=/dev/ttyACM0
# Click & forget arduino IDE should also deal with mcu flashing.

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

UPLOAD_SPEED ?= 115200
UPLOAD_PROTOCOL ?= stk500v1
UPLOAD_PORT ?= /dev/ttyUSB0

# SET this path in same manner - to fit your destination
ARDUINO_PATH = $(HOME)/arduino-1.6.13/hardware/arduino/avr/
# COMMENT these two paths if you don't need to use mightycore, otherwise SET properly
ARDUINO_CORE_PATH = $(HOME)/.arduino15/packages/MightyCore/hardware/avr/2.0.1/cores/MCUdude_corefiles/
ARDUINO_VARIANT_PATH = $(HOME)/.arduino15/packages/MightyCore/hardware/avr/2.0.1/variants/standard/


ARDUINO_CORE_PATH ?= $(ARDUINO_PATH)/cores/arduino
ARDUINO_VARIANT_PATH ?= $(ARDUINO_PATH)/variants/standard/
CORE_LIB_SRC =  \
	$(ARDUINO_CORE_PATH)/main.cpp \
	$(ARDUINO_CORE_PATH)/wiring.c \
	$(ARDUINO_CORE_PATH)/hooks.c 

#avr gcc - if you have avr toolchain installed on system just remove name prefix path
CC = $(ARDUINO_PATH)../../tools/avr/bin/avr-gcc 
#avr objectcopy
OBJCOPY = $(ARDUINO_PATH)../../tools/avr/bin/avr-objcopy
UPLOADER ?= $(ARDUINO_PATH)../../tools/avr/bin/avrdude

CFLAGS = -w -std=gnu11 -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -DF_CPU=$(F_CPU) -DMCU=$(MCU) -I$(ARDUINO_CORE_PATH) -I$(ARDUINO_VARIANT_PATH)

SIMULFLAGS = -Wl,--section-start=.siminfo=0x900000 -DSIMINFO=true
CFLAGS += -mmcu=$(MCU) $(SIMULFLAGS)

#debug?
#CFLAGS += -g
OBJFLAG = -O ihex


EXECUTABLE = teathimble
SOURCES = ${wildcard *.c $(CORE_LIB_SRC)}
HEADERS = ${wildcard *.h}
OBJECTS = ${SOURCES:.c=.o}

.PHONY: all
all: ${EXECUTABLE}

$(EXECUTABLE): $(OBJECTS) buildnumber.num
	$(CC) $(CFLAGS)  -o $(EXECUTABLE).elf $(OBJECTS) -lm
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
	
flash: ${EXECUTABLE}
	$(UPLOADER) -C $(ARDUINO_PATH)../../tools/avr/etc/avrdude.conf -v -p$(MCU) -carduino -P$(UPLOAD_PORT) -b$(UPLOAD_SPEED) -D -Uflash:w:$(EXECUTABLE).hex:i 
