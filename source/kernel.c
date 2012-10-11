#include "kernel.h"

void sh();
static void set_timer(void (*scheduler)());

res_t * res;

void main()
{
    asm "mov ax, cs";
    asm "mov ss, ax";
    asm "mov sp, #0";
    task_sysinit();
    task_create(shell, KERNELBASE);
    res = res_init(1);
    set_timer(task_schedule_irq);
    task_resume(task_get());
}

void set_timer(void (*scheduler)())
{
    scheduler = scheduler;
    asm "push ds";
    asm "mov ax, #0";
    asm "mov ds, ax";
    asm "mov ax, 4[bp]";
    asm "mov word [0x08 * 4], ax";
    asm "mov word [0x08 * 4 + 2], cs";
    asm "pop ds";
}
