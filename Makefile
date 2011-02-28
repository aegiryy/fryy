AS=nasm
CC=bcc
LD=ld86

all: boot.img os.bin

boot.img: boot.nasm
	$(AS) -o boot.img boot.nasm
	dd if=/dev/zero of=boot.img bs=512 count=2878 seek=2

os.o: os.c
	$(CC) -0 -ansi -c os.c

os.bin: os.ld os.o
	$(LD) -d -Tos.ld -M os.o -o os.bin 
# os.bin: os.nasm
# 	$(AS) -o os.bin os.nasm

clean:
	rm boot.img
	rm os.bin
	rm os.o
