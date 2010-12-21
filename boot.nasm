%macro PRINT 1    
    mov ah, 0eh
    mov al, %1
    mov bl, 1110b
    int 10h
%endmacro

BaseOfLoader    equ 0900h
OffsetOfLoader  equ 0100h
RootDirSectors  equ 14
FirstRootSector equ 19

org 0x7c00
jmp short start
nop

; FAT12 header
BS_OEMName      db  "FRYY0.1", 0
BPB_BytsPerSec  dw  0x200
BPB_SecPerClus  db  0x1
BPB_RsvdSecCnt  dw  0x1 ; Boot Sector Count
BPB_NumFATs     db  0x2
BPB_RootEntCnt  dw  0xe0
BPB_TotSec16    dw  0xb40
BPB_Media       db  0xf0
BPB_FATSz16     dw  0x9
BPB_SecPerTrk   dw  0x12
BPB_NumHeads    dw  0x2
BPB_HiddSec     dd  0x0
BPB_TotSec32    dd  0x0
BS_DrvNum       db  0x0
BS_Reserved1    db  0x0
BS_BootSig      db  0x29
BS_VolID        dd  0x0
BS_VolLab       db  "NO NAME   ", 0
BS_FileSysType  db  "FAT12  ", 0   

start:
    mov ax, cs
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov sp, 0x7c00
    ; init A:\
    xor ah, ah
    xor dl, dl
    int 13h
    ; clear screen
    call clear
    ; read sector test
    mov ax, BaseOfLoader
    mov es, ax
    mov bx, OffsetOfLoader
    mov ax, 1
    mov cl, 1
    call read
    ; test
    cmp byte [es:bx], 0xf0
    jne .end
    PRINT 'Y'
.end:
    PRINT 'N'
    jmp $

read:
; ax -> sector index
; es:bx -> buffer pointer
; cl -> sector numbers
    ; save essential values
    mov byte [.count], cl
    push bx
    ; DIV
    mov bl, byte [BPB_SecPerTrk]
    div bl
    ; start sector
    inc ah
    mov cl, ah 
    ; tractor number
    mov ch, al
    shr ch, 1 
    ; header number
    mov dh, al
    and dh, 1
    ; driver number
    mov dl, byte [BS_DrvNum]
    pop bx
.redo:
    mov ah, 2
    mov al, byte [.count]
    int 13h
    jc .redo
    ret
.count db 0

clear:
    mov ax, 0600h
    mov bh, 07
    mov cx, 0000
    mov dx, 184fh
    int 10h
    mov ah, 2
    mov bh, 0
    mov dx, 0
    int 10h
    ret
    

times 510-($-$$) db 0
dw 0xaa55
db 0xf0, 0xff, 0xff
times 1024-($-$$) db 0
