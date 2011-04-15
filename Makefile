AS=nasm
CC=bcc
LD=ld86
INC=include
SRC=source
CFLAGS=-0 -I$(INC) -ansi -c

all: boot.img os.bin

boot.img: $(SRC)/boot.nasm
	$(AS) -o $@ $(SRC)/boot.nasm
	dd if=/dev/zero of=$@ bs=512 count=2879 seek=1

kernel.o: $(SRC)/kernel.c
	$(CC) $(CFLAGS) $(SRC)/kernel.c -o $@ 

task.o: $(SRC)/task.c
	$(CC) $(CFLAGS) $(SRC)/task.c -o $@

io.o: $(SRC)/io.c
	$(CC) $(CFLAGS) $(SRC)/io.c -o $@

os.bin: kernel.o task.o io.o
	$(LD) -d -M kernel.o task.o io.o -o $@
# os.bin: os.nasm
# 	$(AS) -o os.bin os.nasm

clean:
	rm -f boot.img
	rm -f os.bin
	rm -f *.o
