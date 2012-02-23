GCCFLAGS=-g -Os -Wall -mmcu=atmega168 
LINKFLAGS=-Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm
AVRDUDEFLAGS=-c avr109 -p m168 -b 115200 -P COM4
LINKOBJECTS=../libnerdkits/delay.o ../libnerdkits/lcd.o ../libnerdkits/uart.o

all:	sounds-upload

sounds.hex:	sounds.c
	make -C ../libnerdkits
	avr-gcc ${GCCFLAGS} ${LINKFLAGS} -o sounds.o sounds.c ${LINKOBJECTS}
	avr-objcopy -j .text -O ihex sounds.o sounds.hex
	
sounds.ass:	sounds.hex
	avr-objdump -S -d sounds.o > sounds.ass
	
sounds-upload:	sounds.hex
	avrdude ${AVRDUDEFLAGS} -U flash:w:sounds.hex:a
