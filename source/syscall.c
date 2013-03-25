#include "syscall.h"

void *syscall_table[] = {
    task_remove,
    task_get
};

static int _IP, _CS, _FLAG;

/* AX saves return value */
static void syscall_irq()
{
    asm "pop word [__IP]";
    asm "pop word [__CS]";
    asm "pop word [__FLAG]";
    asm "xor bx, bx";
    asm "add ax, ax";
    asm "add bx, ax";
    asm "mov bx, [bx+_syscall_table]";
    asm "call bx";
    asm "push word [__FLAG]";
    asm "push word [__CS]";
    asm "push word [__IP]";
    asm "iret";
}

static void syscall_set_handler(void (*handler)())
{
    handler = handler;
    asm "push ds";
    asm "mov ax, #0";
    asm "mov ds, ax";
    asm "mov ax, 4[bp]";
    asm "mov word [0x21 * 4], ax";
    asm "mov word [0x21 * 4 + 2], cs";
    asm "pop ds";
}

void syscall_sysinit()
{
    syscall_set_handler(syscall_irq);
}

