%macro set_handler 2
    push ds
    mov ax, 0
    mov ds, ax
    mov word [%1 * 4], %2
    mov word [%1 * 4 + 2], cs
    pop ds
%endmacro

    org 0100h
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov word [IP1], task1
    mov word [IP2], task2
    call clear
    set_handler 08h, timer
    jmp task1

timer:
    cmp byte [.flg], 0
    jne .1
.0:
    inc byte [.flg]
    pop word [IP1]
    push word [IP2]
    ; pop word [IP1]
    ; push word [IP1]
    jmp .end
.1:
    dec byte [.flg]
    pop word [IP2]
    push word [IP1]
    ; pop word [IP1]
    ; push word [IP1]
.end:
    mov al, 0x20
    out 0x20, al
    iret
.flg db 0

task1:
.loop:
    mov ah, 0eh
    mov al, 'A'
    mov bl, 1110b
    int 10h
    jmp .loop

task2:
.loop:
    sti
    mov ah, 0eh
    mov al, 'B'
    mov bl, 1110b
    int 10h
    jmp .loop

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

IP1 dw 0
IP2 dw 0

    times 510-($-$$) db 0
    dw 0aa55h
