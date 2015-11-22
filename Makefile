CC=gcc
bin=dotbox
CFLAGS=-O2 -std=c89 -pedantic #-Wall -Werror
.PHONY: all clean

all: $(bin)

$(bin):	main.o  dotbox.o dotbox_io.o function.o opt.o
	$(CC) -o $(bin) main.o  dotbox.o dotbox_io.o function.o opt.o

clean:
	rm -rf *.o *.exe $(bin)


dotbox.o: dotbox.c dotbox.h
	$(CC) $(CFLAGS) -c dotbox.c #-D_COMNAME_ # for better random numbers 

dotbox_io.o: dotbox_io.c dotbox.h dotbox_io.h

main.o: main.c dotbox.h  # -D_DEVRAND_ # for better random numbers

function.o: function.c function.h

opt.o: opt.c opt.h

