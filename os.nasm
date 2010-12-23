start:
    ; init
    mov ax, cs
    mov ds, ax
    mov es, ax
    call disp
    jmp $
disp:
    mov ax, .msg
    mov bp, ax
    mov cx, 13
    mov ax, 01301h
    mov bx, 000ch
    mov dx, 0
    int 10h
    ret
.msg db 'Hello, world!'

times 0x8f600-($-$$) db 3

