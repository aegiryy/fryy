%macro PRINT 1    
    mov ah, 0eh
    mov al, %1
    mov bl, 1110b
    int 10h
%endmacro

org 9100h
    PRINT 'F'
    PRINT 'i'
    PRINT 'n'
    PRINT 'i'
    PRINT 's'
    PRINT 'h'
    PRINT 'e'
    PRINT 'd'
    jmp $
