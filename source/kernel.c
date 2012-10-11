#include "kernel.h"

void sh();
static void set_timer(void (*scheduler)());

void TA();
void TB();
void TC();

res_t * res;

void main()
{
    asm "mov ax, cs";
    asm "mov ss, ax";
    asm "mov sp, #0";
    task_sysinit();
    task_create(shell, KERNELBASE);
    //task_create(TA, KERNELBASE);
    //task_create(TB, KERNELBASE);
    //task_create(TC, KERNELBASE);
    res = res_init(1);
    set_timer(task_schedule_irq);
    task_resume(task_get());
}

void TA()
{
    unsigned int n = 30000;
    P(res);
    while(n--)
        putc('a');
    V(res);
    task_remove(task_get());
}
void TB()
{
    unsigned int n = 30000;
    P(res);
    while(n--)
        putc('b');
    V(res);
    task_remove(task_get());
}
void TC()
{
    unsigned int n = 30000;
    P(res);
    while(n--)
        putc('c');
    V(res);
    task_remove(task_get());
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
