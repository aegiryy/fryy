CC=nasm

all: boot.img os.bin

boot.img: boot.nasm
	$(CC) -o boot.img boot.nasm
	dd if=/dev/zero of=boot.img bs=512 count=2878 seek=2

os.bin: os.nasm
	$(CC) -o os.bin os.nasm

clean:
	rm boot.img
	rm os.bin
