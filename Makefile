CC=gcc
bin1=dotbox
bin2=fvj
CFLAGS=-O2 -std=c89 -pedantic -Wall -Werror # -D_DEVRAND_ # for better random numbers
.PHONY: all clean

all: $(bin1) $(bin2)

$(bin1):	main.o  dotbox.o dotbox_io.o function.o opt.o
	$(CC) -o $(bin1) main.o  dotbox.o dotbox_io.o function.o opt.o
	
$(bin2):	fvj.o  dotbox.o dotbox_io.o function.o opt.o
	$(CC) -o $(bin2) fvj.o  dotbox.o dotbox_io.o function.o opt.o
	
clean:
	rm -rf *.o *.exe $(bin1) $(bin2)


dotbox.o: dotbox.c dotbox.h
	$(CC) $(CFLAGS) -c dotbox.c

dotbox_io.o: dotbox_io.c dotbox.h dotbox_io.h

main.o: main.c dotbox.h

function.o: function.c function.h

opt.o: opt.c opt.h

fvj.o: fvj.c dotbox.h dotbox_io.h function.h
