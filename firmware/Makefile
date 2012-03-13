PART=atmega328p
PROGRAMMER = avrispmkII -P usb -e


SRC=vfd.c
HEX=${SRC:.c=.hex}
OBJ=${SRC:.c=.o}


CC=avr-gcc
CFLAGS=-Os -Wall -pedantic-errors -Werror -mcall-prologues -mmcu=${PART} -std=c99
OBJ2HEX=avr-objcopy 

AVRDUDE = avrdude -c ${PROGRAMMER} -p ${PART}

default: clean program

program: ${HEX}
	$(AVRDUDE) -e -U flash:w:${HEX}

rdfuses:
	$(AVRDUDE) -B 100 -v -q

# see: http://www.engbedded.com/fusecalc/
fuses:
	$(AVRDUDE) -B 100 -u -v -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

%.obj : ${SRC} Makefile
	$(CC) $(CFLAGS) -DNODE=$(NODE) ${SRC} -o $@
	avr-size -C $@

%.hex : %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

clean :
	rm -f *.hex *.obj *.o *.eep


