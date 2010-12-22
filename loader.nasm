%macro PRINT 1    
    mov ah, 0eh
    mov al, %1
    mov bl, 1110b
    int 10h
%endmacro

start:
    ; init
    mov ax, cs 
    mov ds, ax
    mov es, ax
    PRINT 'O'
    PRINT 'K'
    jmp $

times 0x11000-($-$$) db 3

