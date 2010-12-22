CC=nasm

all: boot.img loader.bin

boot.img: boot.nasm
	$(CC) -o boot.img boot.nasm
	dd if=/dev/zero of=boot.img bs=512 count=2878 seek=2

loader.bin: loader.nasm
	$(CC) -o loader.bin loader.nasm
