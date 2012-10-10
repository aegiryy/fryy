#include "kernel.h"

void sh();
void init();
static void set_timer(void (*scheduler)());

void TA();
void TB();
void TC();

//res_t * res;

void main()
{
    asm "mov ax, cs";
    asm "mov ss, ax";
    asm "mov sp, #0";
    task_sysinit();
    //task_create(init, KERNELBASE);
    task_create(TA, KERNELBASE);
    task_create(TB, KERNELBASE);
    task_create(TC, KERNELBASE);
    set_timer(task_schedule_irq);
    task_resume(task_get());
}

/* root task */
void init()
{
    task_create(shell, KERNELBASE);
    task_remove(task_get());
}

void TA()
{
    while(1)
        putc('a');
}
void TB()
{
    while(1)
        putc('b');
}
void TC()
{
    while(1)
        putc('c');
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
