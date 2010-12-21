CC=nasm

all: boot

boot: boot.nasm
	$(CC) boot.nasm
	dd if=/dev/zero of=boot bs=512 count=2878 seek=2
