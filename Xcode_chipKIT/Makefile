export PATH := $(PATH):/Applications/Mpide.app/Contents/Resources/Java/hardware/pic32/compiler/pic32-tools/bin/

F_CPU=72000000L
#F_CPU=96000000L

# normal baudrate for ftdi comm at 72mhz = 1500000

DEFINES=-DBAUDRATE=230400 -D__C32__ 

TARGET=main

AS=pic32-as
CC=pic32-gcc
CXX=pic32-g++
LD=pic32-ld
OBJDUMP=pic32-objdump
OBJCOPY=pic32-objcopy
BIN2HEX=pic32-bin2hex
SIZE=pic32-size

LDSCRIPT=/Applications/Mpide.app/Contents/Resources/Java/hardware/pic32/cores/pic32/chipKIT-UNO32-application-32MX320F128L.ld

AVRDUDE=/Applications/Mpide.app/Contents/Resources/Java/hardware/tools/avr/bin/avrdude

CBASE=-O2 -fno-exceptions -mno-smart-io -mprocessor=32MX320F128H -DF_CPU=$(F_CPU) -D_BOARD_MEGA_ $(DEFINES)

CFLAGS=$(WARNINGSC) $(CBASE) $(INCLUDE)
CXXFLAGS=$(WARNINGSCXX) $(CBASE) $(INCLUDE) -fno-exceptions -fno-rtti 
ASFLAGS=-save-temps

LINKER_FLAGS= -Xlinker -T$(LDSCRIPT) -Xlinker -o$(OUTPUT).elf -Xlinker -M -Xlinker -Map=$(OUTPUT).map 

INCLUDES=/Applications/Mpide.app/Contents/Resources/Java/hardware/pic32/cores/pic32 source source/wiring source/USB
SOURCES = source source/wiring source/USB crt0

BUILD   = build

ifneq ($(BUILD),$(notdir $(CURDIR)))

export LDSCRIPT=$(CURDIR)/lpc2104-rom2.ld
export MAKEFILE = $(CURDIR)/Makefile

export INCLUDE = $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
    $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
    -I$(CURDIR)/$(BUILD)

export OUTPUT   :=  $(CURDIR)/$(TARGET)
export VPATH    :=  $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
            $(foreach dir,$(DATA),$(CURDIR)/$(dir))
export DEPSDIR  :=  $(CURDIR)/$(BUILD)

#export CRT0=/Applications/Mpide.app/Contents/Resources/Java/hardware/pic32/cores/pic32/*.S
#export CRT0=source/wiring/*.S

export CFILES   = $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
export SFILES   = $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
export CPPFILES = $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
export AUTOSRC = $(AUTOC) $(AUTOCPP)

export THUMB_SRC = $(AUTOSRC)

export OFILES   = $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.S=.o)

.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@#@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	@make -C $(BUILD) -f $(MAKEFILE)

all:	$(BUILD)

clean:
	rm -rf $(BUILD) $(TARGET).elf $(TARGET).{hex,map,elf} html

burn:	$(BUILD)
	$(AVRDUDE) -C/Applications/Mpide.app/Contents/Resources/Java/hardware/tools/avr/etc/avrdude.conf -q -q -p32MX320F128HL -cstk500v2 -P/dev/tty.usbserial-A5004FII -b115200 -D -Uflash:w:$(OUTPUT).hex:i

else

DEPENDS :=  $(OFILES:.o=.d)

$(OUTPUT).hex:  $(OUTPUT).elf

$(OUTPUT).elf:  $(OFILES)

-include $(DEPENDS)

endif

$(OUTPUT).hex:	$(OUTPUT).elf
	$(OBJCOPY) $(OUTPUT).elf -O ihex $(OUTPUT).hex

$(OUTPUT).elf:	$(OFILES) $(CRT0) $(MAKEFILE)
	$(CXX) -c $(OFILES) -Wl,--gc-sections -mprocessor=32MX320F128H $(LINKER_FLAGS)

%.o:	%.c
	@echo $(notdir $<)
	$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) -c $< -o $@ $(ERROR_FILTER)