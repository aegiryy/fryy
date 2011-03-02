AS=nasm
CC=bcc
LD=ld86
INC=include
SRC=source
CFLAGS=-0 -I$(INC) -ansi -c

all: boot.img os.bin

boot.img: $(SRC)/boot.nasm
	$(AS) -o boot.img $(SRC)/boot.nasm
	dd if=/dev/zero of=boot.img bs=512 count=2878 seek=2

os.o: $(SRC)/os.c
	$(CC) $(CFLAGS) $(SRC)/os.c -o os.o

task.o: $(SRC)/task.c
	$(CC) $(CFLAGS) $(SRC)/task.c -o task.o

io.o: $(SRC)/io.c
	$(CC) $(CFLAGS) $(SRC)/io.c -o io.o

os.bin: os.o task.o io.o
	$(LD) -d -M os.o task.o io.o -o os.bin 
# os.bin: os.nasm
# 	$(AS) -o os.bin os.nasm

clean:
	rm boot.img
	rm os.bin
	rm *.o
