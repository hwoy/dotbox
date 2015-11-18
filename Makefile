CC=gcc
bin=dotbox
CFLAGS=-O2 -std=c89 -pedantic
.PHONY: all clean

all: $(bin)

$(bin):	main.o  dotbox.o dotbox_io.o
	$(CC) -o $(bin) main.o  dotbox.o dotbox_io.o

clean:
	rm -rf *.o *.exe $(bin)


dotbox.o: dotbox.c dotbox.h
dotbox_io.o: dotbox_io.c dotbox.h dotbox_io.h
main.o: main.c dotbox.h

