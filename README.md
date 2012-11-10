# fryy #

## OVERVIEW ##
1. Get deep understanding of INTEL 8086 architecture
2. Build a micro OS kernel including 
   * FAT12 Filesystem support
   * Process Management (PM) with Multi-Task support
   * System call support

## ROADMAP ##
1. Understanding BOOT procedure of 8086 system (FLOPPY only!)
    * Creating DEV environment -- [QEMU](http://qemu.org), an emulator 
      that can provides virtual environments of 80386, x86_64, ARM, 
      MIPS, SPARC, etc.
2. Getting familar with INTEL 8086 ISA 
    * Read INTEL 8086 Manual
    * Download & Install [NASM](http://www.nasm.us/pub/nasm/releasebuilds/2.10.05/)
    * Read [NASM's manual](http://www.nasm.us/xdoc/2.09.04/nasmdoc.pdf)
3. Understanding FAT12 specifications
    * Create a virtual floppy image (boot.img, source: [boot.nasm](https://github.com/aegiryy/fryy/blob/master/source/boot.nasm))
4. Build a bootloader of our future OS
    * Understanding the limit of floppy boot: code size limit of 512B
    * BootLoader construction process:
      * Search KERNEL.BIN in RootDirSectors
      * Read File Allocation Table (FAT) for details
      * Loading KERNEL.BIN using BIOS INT (0x13)
      * Transfer control to KERNEL.BIN
5. Build KERNEL.BIN
    * Download Bruce's C Compiler (bcc)

            bcc -0 -ansi -c source.c   
                -0:     generating 8086 (16bit) code    
                -ansi:  support ANSI C    
                -c:     compile only    

            ld86 -d -M source.o -o source.bin   
                -d:     generate flat binary file    
                -M:     print organization    
                -o:     target file    
      See manuals for details of bcc, as86, ld86.
    * Build a simple process management system
      * Task scheduling subsystem (supporting multiprocesses)
      * A basic IPC subsystem (PV operators && resources)
    * Write a simple SHELL process dealing with I/O  
      TIPS: Library functions (e.g. strcmp) can be linked to target image

## HOW TO RUN IT? ##
1. Install QEMU & BCC (including bcc, as86 and ld86)
2. make
3. Put os.bin to boot.img (e.g. under Linux).
    * mkdir /mnt/floppy/
    * mount -o loop boot.img /mnt/floppy/
    * cp os.bin /mnt/floppy/  
    * umount /mnt/floppy/
4. qemu -fda boot.img
