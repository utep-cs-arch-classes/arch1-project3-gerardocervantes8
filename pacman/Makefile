



# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h -I../h

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:pacman.elf

#additional rules for files
pacman.elf: ${COMMON_OBJECTS} pacman.o wdt_handler.o buzzer.o sound.o movingLayer.o enemy.o 
	${CC} -mmcu=${CPU}   -o $@ $^ -L../lib -lTimer -lLcd -lShape -lCircle -lp2sw

pacman.o: pacman.h movingLayer.h

buzzer.o: buzzer.h

sound.o: #sound.h

#enemy.o: enemy.h

movingLayer.o: #movingLayer.h

load: pacman.elf
	mspdebug rf2500 "prog $^"

clean:
	rm -f *.o *.elf
