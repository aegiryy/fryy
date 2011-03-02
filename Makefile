AS=nasm
CC=bcc
LD=ld86
INC=include
SRC=source

all: boot.img os.bin

boot.img: $(SRC)/boot.nasm
	$(AS) -o boot.img $(SRC)/boot.nasm
	dd if=/dev/zero of=boot.img bs=512 count=2878 seek=2

os.o: $(SRC)/os.c
	$(CC) -0 -I$(INC) -ansi -c $(SRC)/os.c -o os.o

os.bin: os.o
	$(LD) -d -M os.o -o os.bin 
# os.bin: os.nasm
# 	$(AS) -o os.bin os.nasm

clean:
	rm boot.img
	rm os.bin
	rm *.o
