###############################################################################
# Makefile for the project AVR_STUDIO
###############################################################################

## General Flags
PROJECT = AVR_STUDIO
MCU = atmega16
TARGET = AVR_STUDIO.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=AVR_STUDIO.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = main.o adc.o button.o diskio.o ds1307.o lcd-lib.o pff.o spi_2.o timer-2.o timer.o twi.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) AVR_STUDIO.hex AVR_STUDIO.eep AVR_STUDIO.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

adc.o: ../adc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

button.o: ../button.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

diskio.o: ../diskio.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ds1307.o: ../ds1307.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd-lib.o: ../lcd-lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

pff.o: ../pff.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

spi_2.o: ../spi_2.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer-2.o: ../timer-2.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer.o: ../timer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

twi.o: ../twi.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) AVR_STUDIO.elf dep/* AVR_STUDIO.hex AVR_STUDIO.eep AVR_STUDIO.lss AVR_STUDIO.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

