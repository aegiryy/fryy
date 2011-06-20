! 1 
! 1 # 1 "source/filesystem.c"
! 1 # 3 "include/filesystem.h"
! 3 typedef struct _file_t
! 4 {
! 5     char name[11];
!BCC_EOS
! 6     char attr;
!BCC_EOS
! 7     int fstCluster;
!BCC_EOS
! 8     int filesize[2];
!BCC_EOS
! 9 } file_t;
!BCC_EOS
! 10 # 3 "source/filesystem.c"
! 3 static void load_sectors();
!BCC_EOS
! 4 
! 5 static void load_sectors(addr,sector_no,count)
! 6 # 5 "source/filesystem.c"
! 5 char * addr;
_load_sectors:
!BCC_EOS
! 6 # 5 "source/filesystem.c"
! 5 int sector_no;
!BCC_EOS
! 6 # 5 "source/filesystem.c"
! 5 char count;
!BCC_EOS
! 6 {
! 7     sector_no = sector_no;
push	bp
mov	bp,sp
push	di
push	si
! Debug: eq int sector_no = [S+6+4] to int sector_no = [S+6+4] (used reg = )
!BCC_EOS
! 8     count = 1;
! Debug: eq int = const 1 to char count = [S+6+6] (used reg = )
mov	al,*1
mov	8[bp],al
!BCC_EOS
! 9     asm "mov es, #0x1000";
!BCC_ASM
	mov es, #0x1000
!BCC_ENDASM
!BCC_EOS
! 10     asm "mov bx, 4[bp]";
!BCC_ASM
	mov bx, 4[bp]
!BCC_ENDASM
!BCC_EOS
! 11     asm "push bx";
!BCC_ASM
	push bx
!BCC_ENDASM
!BCC_EOS
! 12     asm "mov ax, 6[bp]";
!BCC_ASM
	mov ax, 6[bp]
!BCC_ENDASM
!BCC_EOS
! 13     asm "mov bl, #18";
!BCC_ASM
	mov bl, #18
!BCC_ENDASM
!BCC_EOS
! 14     asm "div bl";
!BCC_ASM
	div bl
!BCC_ENDASM
!BCC_EOS
! 15     asm "inc ah";
!BCC_ASM
	inc ah
!BCC_ENDASM
!BCC_EOS
! 16     asm "mov cl, ah";
!BCC_ASM
	mov cl, ah
!BCC_ENDASM
!BCC_EOS
! 17     asm "mov ch, al";
!BCC_ASM
	mov ch, al
!BCC_ENDASM
!BCC_EOS
! 18     asm "shr ch, #1";
!BCC_ASM
	shr ch, #1
!BCC_ENDASM
!BCC_EOS
! 19     asm "mov dh, al";
!BCC_ASM
	mov dh, al
!BCC_ENDASM
!BCC_EOS
! 20     asm "and dh, #1";
!BCC_ASM
	and dh, #1
!BCC_ENDASM
!BCC_EOS
! 21     asm "mov dl, #0";  
!BCC_ASM
	mov dl, #0
!BCC_ENDASM
!BCC_EOS
! 22     asm "pop bx";
!BCC_ASM
	pop bx
!BCC_ENDASM
!BCC_EOS
! 23     asm "mov ah, #2";
!BCC_ASM
	mov ah, #2
!BCC_ENDASM
!BCC_EOS
! 24     asm "mov al, #1";
!BCC_ASM
	mov al, #1
!BCC_ENDASM
!BCC_EOS
! 25     asm "int 0x13";
!BCC_ASM
	int 0x13
!BCC_ENDASM
!BCC_EOS
! 26 }
pop	si
pop	di
pop	bp
ret
! 27 
.data
.bss

! 0 errors detected
