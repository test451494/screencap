#
#
#
#
CC = gcc
CFLAGS = -g -Wall
INC = -I./include
LIB = -lpthread -ljpeg

screencap: screencap.o 
	${CC} -o ./bin/screencap screencap.o ${LIB} 

program.o: program.c 
	${CC} -c ${CFLAGS} screencap.c


clean:
	rm -rf screencap.o
