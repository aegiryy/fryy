#include "kernel.h"

void task1();
void task2();
void task3();
static void set_timer(void (*scheduler)());
static void scheduler();

res_t * res;

void main()
{
    asm "mov ax, cs";
    asm "mov ss, ax";
    asm "mov sp, #0";
    task_init(task1, 0x1000);
    task_init(task2, 0x1000);
    task_init(task3, 0x1000);
    res = res_init(2);
    set_timer(task_schedule);
    task_set(task_get());
}

void task1()
{
    int s = 0x7fff;
    P(res);
    while (s--)
    {
        putc('A');
    }
    V(res);
    task_deinit(task_get());
}

void task2()
{
    int s = 0x7fff;
    P(res);
    while (s--)
    {
        putc('B');
    }
    V(res);
    task_deinit(task_get());
}

void task3()
{
    int s = 0x7fff;
    P(res);
    while (s--)
    {
        putc('C');
    }
    V(res);
    task_deinit(task_get());
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
