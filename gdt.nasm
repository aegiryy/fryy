%macro descr 3
    dw %2 & 0xffff
    dw %1 & 0xffff
    db (%1 >> 16) & 0xff
    dw (0xf0ff & %3) | (0x0f00 & (%2 >> 8))
    db %1 >> 24
%endmacro

DA_32 equ 0x4000
DA_C equ 0x98
DA_DRW equ 0x92

org 0x7c00
    jmp begin

[section .gdt]
LABEL_GDT   descr   0, 0, 0
LABEL_DESCR_CODE32 descr 0, SegCode32Len - 1, DA_C | DA_32
LABEL_DESCR_VIDEO descr 0xb8000, 0xffff, DA_DRW

GdtLen  equ $ - LABEL_GDT
GdtPtr  dw  GdtLen - 1
        dd  0

SelectorCode32  equ LABEL_DESCR_CODE32 - LABEL_GDT
SelectorVideo   equ LABEL_DESCR_VIDEO - LABEL_GDT

[section .s16]
[bits 16]
begin:
    ; initializing
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x0100
    ; fill code GDT entry
    xor eax, eax
    mov ax, cs
    shl eax, 4
    add eax, LABEL_SEG_CODE32
    mov word [LABEL_DESCR_CODE32 + 2], ax
    shr eax, 16
    mov byte [LABEL_DESCR_CODE32 + 4], al
    mov byte [LABEL_DESCR_CODE32 + 7], ah
    ; fill GdtPtr for loading
    xor eax, eax
    mov ax, ds
    shl eax, 4
    add eax, LABEL_GDT
    mov dword [GdtPtr + 2], eax

    lgdt [GdtPtr]

    cli
    in al, 0x92
    or al, 00000010b
    out 0x92, al

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp dword SelectorCode32:0

[section .s32]
[bits 32]
LABEL_SEG_CODE32:
    mov ax, SelectorVideo
    mov gs, ax
    mov edi, (80 * 11 + 79) * 2
    mov ah, 0x0c
    mov al, 'P'
    mov [gs:edi], ax
    jmp $
SegCode32Len equ $ - LABEL_SEG_CODE32
