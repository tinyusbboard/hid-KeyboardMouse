#######################################################################################

# environment variable of the current user to locate the AVR8 toolchain
AVRPATH = $(AVR8TOOLCHAINBINDIR)

# the type of avr microcontroller
DEVICE = atmega8

# the frequency the microcontroller is clocked with
F_CPU = 16000000

#tinyUSBboard Rev3 and newer
DEFINES += -DVUSB_CFG_IOPORTNAME=D -DVUSB_CFG_DMINUS_BIT=7 -DVUSB_CFG_DPLUS_BIT=2
DEFINES += -DVUSB_CFG_HASNO_PULLUP_IOPORTNAME -DVUSB_CFG_HASNO_PULLUP_BIT

# extra defines
DEFINES += -DUSB_CFG_HID_WHEELMOUSE
# DEFINES += -DUSB_CFG_HID_NOMOUSE
# DEFINES += -DUSB_CFG_HID_NOKEYBOARD

# where the firmware should be located within the flashmemory (in case you trampoline)
FLASHADDRESS = 0x0000

# (not important for compiling) - the device transporting firmware into the controller
PROGRAMMER = -c usbasp

#######################################################################################



# Tools:
ECHO=@echo
GCC=gcc
MAKE=@make
RM=@rm -f

CC=$(AVRPATH)avr-gcc
OBC=@$(AVRPATH)avr-objcopy
OBD=@$(AVRPATH)avr-objdump
SIZ=@$(AVRPATH)avr-size

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)


MYCFLAGS = -Wall -Os -fno-move-loop-invariants -fno-tree-scev-cprop -fno-inline-small-functions -I. -Iusbdrv -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) $(CFLAGS)   $(DEFINES)
MYLDFLAGS = -Wl,--relax,--gc-sections $(LDFLAGS)


FLASHPREAMBLEDEFINE = 
ifneq ($(FLASHADDRESS), 0)
ifneq ($(FLASHADDRESS), 00)
ifneq ($(FLASHADDRESS), 000)
ifneq ($(FLASHADDRESS), 0000)
ifneq ($(FLASHADDRESS), 00000)
ifneq ($(FLASHADDRESS), 0x0)
ifneq ($(FLASHADDRESS), 0x00)
ifneq ($(FLASHADDRESS), 0x000)
ifneq ($(FLASHADDRESS), 0x0000)
ifneq ($(FLASHADDRESS), 0x00000)
FLASHPREAMBLE = 0x0000
FLASHPREAMBLEDEFINE = -DFLASHPREAMBLE=$(FLASHPREAMBLE)
MYLDFLAGS += -Wl,--section-start=.text=$(FLASHADDRESS)
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif




STDDEP	 = *.h usbdrv/*.h
EXTRADEP = Makefile


all:  main.hex main.raw main.asm

usbdrv/usbdrvasm.o: usbdrv/usbdrvasm.S $(STDDEP) $(EXTRADEP)
	$(CC) -x assembler-with-cpp -c usbdrv/usbdrvasm.S -o usbdrv/usbdrvasm.o $(MYCFLAGS)

usbdrv/oddebug.o: usbdrv/oddebug.c $(STDDEP) $(EXTRADEP)
	$(CC) usbdrv/oddebug.c -c -o usbdrv/oddebug.o $(MYCFLAGS)

usbdrv/usbdrv.o: usbdrv/usbdrv.c $(STDDEP) $(EXTRADEP)
	$(CC) usbdrv/usbdrv.c -c -o usbdrv/usbdrv.o $(MYCFLAGS)

hidcore.o: hidcore.c $(STDDEP) $(EXTRADEP)
	$(CC) hidcore.c -c -o hidcore.o $(MYCFLAGS)

asciimap.o: asciimap.c $(STDDEP) $(EXTRADEP)
	$(CC) asciimap.c -c -o asciimap.o $(MYCFLAGS)

main.o: main.c $(STDDEP) $(EXTRADEP)
	$(CC) main.c -c -o main.o $(MYCFLAGS)

main.elf: main.o hidcore.o asciimap.o usbdrv/usbdrv.o usbdrv/oddebug.o usbdrv/usbdrvasm.o $(STDDEP) $(EXTRADEP)
	$(CC) main.o hidcore.o asciimap.o usbdrv/usbdrv.o usbdrv/oddebug.o usbdrv/usbdrvasm.o -o main.elf $(MYCFLAGS) -Wl,-Map,main.map $(MYLDFLAGS)
	$(ECHO) "."
	$(SIZ) main.elf
	$(ECHO) "."

main.hex: main.elf $(STDDEP) $(EXTRADEP)
	$(OBC) -j .text -j .data -O ihex main.elf main.hex

main.raw: main.elf $(STDDEP) $(EXTRADEP)
	$(OBC) -j .text -j .data -O binary main.elf main.raw

main.asm: main.elf $(STDDEP) $(EXTRADEP)
	$(OBD) -d main.elf > main.asm

disasm: main.elf $(STDDEP) $(EXTRADEP)
	$(OBD) -d main.elf

flash: all
	$(ECHO) "."
	$(AVRDUDE) -U flash:w:main.hex:i
	$(ECHO) "."

deepclean: clean
	$(RM) usbdrv/*~
	$(RM) *~

clean:
	$(RM) *.o
	$(RM) usbdrv/*.o
	$(RM) main.hex
	$(RM) main.raw
	$(RM) main.asm
	$(RM) main.map
	$(RM) main.elf
