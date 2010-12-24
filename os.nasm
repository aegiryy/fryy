; Set interrupt handler
%macro SETHANDLER 2
    push ds
    mov ax, 0
    mov ds, ax
    mov word [%1 * 4], %2
    mov word [%1 * 4 + 2], cs
    pop ds
%endmacro

; Task Control Block
%macro TCB 4
    dw %1 ; FLAG
    dw %2 ; CS
    dw %3 ; IP
    dw %4 ; Next Task
%endmacro

%macro INITTASK 4
    push %4
    push %3
    push %2
    pushf
    push %1
    call inittask
    add sp, 10
%endmacro 

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
    ; init tasks
    INITTASK tsk_pa, cs, pa, tsk_pb
    INITTASK tsk_pb, cs, pb, tsk_pc
    INITTASK tsk_pc, cs, pc, tsk_pa
    mov word [curtsk], tsk_pa
    SETHANDLER 08h, scheduler
    jmp pa

pa:
.loop:
    PRINT 'A'
    jmp .loop
pb:
.loop:
    PRINT 'B'
    jmp .loop
pc:
.loop:
    PRINT 'C'
    jmp .loop
    
inittask:
; params: tc, flag, cs, ip, nxt
    push bp
    mov bp, sp
    push si
    push ax
    mov si, word [bp + 4]
    mov ax, word [bp + 6]
    mov word [si], ax
    mov ax, word [bp + 8]
    mov word [si + 2], ax
    mov ax, word [bp + 10]
    mov word [si + 4], ax
    mov ax, word [bp + 12]
    mov word [si + 6], ax
    pop ax
    pop si
    pop bp
    ret

scheduler:
    push ax
    push bp
    mov bp, word [curtsk]
    add sp, 4
    pop word [ds:bp + 4]
    pop word [ds:bp + 2]
    pop word [ds:bp]
    mov bp, word [ds:bp + 6]
    mov word [curtsk], bp
    push word [ds:bp]
    push word [ds:bp + 2]
    push word [ds:bp + 4]
    sub sp, 4
    mov al, 0x20
    out 0x20, al
    pop bp
    pop ax
    iret
    
curtsk dw 0

tsk_pa TCB 0, 0, 0, tsk_pb
tsk_pb TCB 0, 0, 0, tsk_pc
tsk_pc TCB 0, 0, 0, tsk_pa

times 0x8f600-($-$$) db 3
