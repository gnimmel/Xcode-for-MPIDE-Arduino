#
# Xcode Makefile for MPIDE Arduino
# Arduino version
#
#
# 2011-11-01 release 1 - initial release for Arduino only
# 2011-11-04 release 2 - for Arduino and chipKIT
# 2011-11-14 release 3 - #include libraries compiled and linked
#
#
# CC = BY NC SA
# http://sites.google.com/site/vilorei/
#
# MPIDE 0022
# Xcode 4.2 on Mac OS X 7.2
#
# Based on so many compiled contributions I can't remember! 
#
#
#

#
# Board configuration
#
MCU = atmega328p # !! updated
F_CPU = 16000000L
FORMAT = ihex
UPLOAD_RATE = 115200 # !! updated


#
# Directories
# Update these to reflect the location of the Arduino IDE and avrdude installations, respectively
#
ARDUINO = /Applications/Mpide.app/Contents/Resources/Java/hardware/arduino/cores/arduino
AVRDUDE_DIR = /Applications/Mpide.app/Contents/Resources/Java/hardware/tools/avr/bin/
AVRDUDE_CONF = /Applications/Mpide.app/Contents/Resources/Java/hardware/tools/avr/etc/avrdude.conf
SOURCES = $(wildcard src/*.cpp) $(wildcard *.cpp)


#
# Libraries
# loading libraries - adjust to your needs
# 
STEPPER = $(LIBRARIES)/Stepper
SERVO = $(LIBRARIES)/Servo
EEPROM = $(LIBRARIES)/EEPROM
INCLUDE_LIBRARIES = -I$(STEPPER) -I$(SERVO) -I$(EEPROM)
CPP_LIBRARIES = $(wildcard $(STEPPER)/*.cpp) $(wildcard $(SERVO)/*.cpp) $(wildcard $(EEPROM)/*.cpp) 


#
# Port and ?
#
PORT = /dev/tty.usbmodem* # !! was PORT = /dev/tty.usbserial*
TARGET = main
SRC = $(ARDUINO)/pins_arduino.c $(ARDUINO)/wiring.c \
$(ARDUINO)/wiring_analog.c $(ARDUINO)/wiring_digital.c \
$(ARDUINO)/wiring_pulse.c \
$(ARDUINO)/wiring_shift.c $(ARDUINO)/WInterrupts.c
CXXSRC = $(ARDUINO)/HardwareSerial.cpp $(ARDUINO)/WMath.cpp $(ARDUINO)/WString.cpp $(ARDUINO)/Print.cpp $(CPP_LIBRARIES) $(SOURCES)

#
# Name of this Makefile (used for "make depend").
#
MAKEFILE = Makefile

#
# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
#
DEBUG = stabs

OPT = s

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU)
CXXDEFS = -DF_CPU=$(F_CPU)

# Place -I options here
CINCS = -I$(ARDUINO)
CXXINCS = -I$(ARDUINO)

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99
CDEBUG = -g$(DEBUG)
CWARN = -Wall -Wstrict-prototypes
CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)

CFLAGS = $(CDEBUG) $(CDEFS) $(CINCS) -O$(OPT) $(CWARN) $(CSTANDARD) $(CEXTRA)
CXXFLAGS = $(CDEFS) $(CINCS) -O$(OPT)
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 
LDFLAGS = -lm


# Programming support using avrdude. Settings and variables.
AVRDUDE_PROGRAMMER = arduino
AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
AVRDUDE_FLAGS = -F -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) -C $(AVRDUDE_CONF) -b $(UPLOAD_RATE)
  

# Program settings
CC = $(AVRDUDE_DIR)/avr-gcc
CXX = $(AVRDUDE_DIR)/avr-g++
OBJCOPY = $(AVRDUDE_DIR)/avr-objcopy
OBJDUMP = $(AVRDUDE_DIR)/avr-objdump
AR  = $(AVRDUDE_DIR)/avr-ar
SIZE = $(AVRDUDE_DIR)/avr-size
NM = $(AVRDUDE_DIR)/avr-nm
AVRDUDE = $(AVRDUDE_DIR)/avrdude
REMOVE = rm -f
MV = mv -f

# Define all object files.
OBJ = $(SRC:.c=.o) $(CXXSRC:.cpp=.o) $(ASRC:.S=.o)

# Define all listing files.
LST = $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst) $(SRC:.c=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS)
ALL_CXXFLAGS = -mmcu=$(MCU) -I. $(CXXFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)


# Default target.
all: build

build: elf hex

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss 
sym: $(TARGET).sym

# Program the device.  
upload: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)




# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
--change-section-address .data-0x800000 \
--change-section-address .bss-0x800000 \
--change-section-address .noinit-0x800000 \
--change-section-address .eeprom-0x810000 


coff: $(TARGET).elf
	$(COFFCONVERT) -O coff-avr $(TARGET).elf $(TARGET).cof


extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr $(TARGET).elf $(TARGET).cof


.SUFFIXES: .elf .hex .eep .lss .sym

.elf.hex:
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

.elf.eep:
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@


core.a: $(OBJ)
	@for i in $(OBJ); do echo $(AR) rcs core.a $$i; $(AR) rcs core.a $$i; done

# Link: create ELF output file from library.
$(TARGET).elf: core.a
	$(CC) $(ALL_CFLAGS) -o $@ $(TARGET).cpp -L. core.a $(LDFLAGS)

# Compile: create object files from C++ source files.
.cpp.o:
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@ 

# Compile: create object files from C source files.
.c.o:
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
.c.s:
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
.S.o:
	$(CC) -c $(ALL_ASFLAGS) $< -o $@



# Target: clean project.
clean:
	$(REMOVE) $(TARGET).hex $(TARGET).eep $(TARGET).cof $(TARGET).elf \
	$(TARGET).map $(TARGET).sym $(TARGET).lss core.a \
	$(OBJ) $(LST) $(SRC:.c=.s) $(SRC:.c=.d) $(CXXSRC:.cpp=.s) $(CXXSRC:.cpp=.d)

depend:
	if grep '^# DO NOT DELETE' $(MAKEFILE) >/dev/null; \
	then \
		sed -e '/^# DO NOT DELETE/,$$d' $(MAKEFILE) > \
			$(MAKEFILE).$$$$ && \
		$(MV) $(MAKEFILE).$$$$ $(MAKEFILE); \
	fi
	echo '# DO NOT DELETE THIS LINE -- make depend depends on it.' \
		>> $(MAKEFILE); \
	$(CC) -M -mmcu=$(MCU) $(CDEFS) $(CINCS) $(SRC) $(ASRC) >> $(MAKEFILE)

.PHONY:	all build elf hex eep lss sym program coff extcoff clean depend
