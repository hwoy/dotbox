CC=gcc
bin=dotbox

.PHONY: all clean

all: $(bin)

$(bin):	main.o  dotbox.o
	$(CC) -o $(bin) main.o  dotbox.o

clean:
	rm -rf *.o *.exe $(bin)
dotbox.o: dotbox.c dotbox.h
main.o: main.c dotbox.h

