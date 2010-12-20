CC=nasm

all: boot.img

boot.img: boot.nasm
	$(CC) -o boot.img boot.nasm
	dd if=/dev/zero of=boot.img bs=512 count=2878 seek=2
