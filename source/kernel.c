#include "kernel.h"
void task1();
void task2();
void task3();
static void set_timer(void (*scheduler)());
static void scheduler();

void main()
{
    asm "mov ax, cs";
    asm "mov sp, ax";
    task_init(task1, 0x1000, 0x0202);
    task_init(task2, 0x1000, 0x0202);
    task_init(task3, 0x1000, 0x0202);
    set_timer(scheduler);
    task_set(curtsk);
}


void task1()
{
    while(1)
    {
        putc('A');
    }
}

void task2()
{
    while(1)
    {
        putc('B');
    }
}

void task3()
{
    while(1)
    {
        putc('C');
    }
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

void scheduler(int cs, int flag)
{
    asm "push ax";
    asm "push bx";
    asm "push cx";
    asm "push dx";
    asm "push bp";
    /* SP should be saved seperatly */
    asm "push si";
    asm "push di";
    asm "push ds";
    asm "push ss";
    asm "push es";
    asm "mov bx, word [_curtsk]";
    asm "pop word 22[bx]";
    asm "pop word 20[bx]";
    asm "pop word 18[bx]";
    asm "pop word 14[bx]";
    asm "pop word 12[bx]";
    asm "pop word 10[bx]";
    asm "pop word 6[bx]";
    asm "pop word 4[bx]";
    asm "pop word 2[bx]";
    asm "pop word [bx]";
    asm "pop word 24[bx]";
    asm "pop word 16[bx]";
    asm "pop word 26[bx]";
    asm "mov word 8[bx], sp";
    /* Context of previous process is saved */

    asm "mov al, #0x20";
    asm "out #0x20, al";
    task_set(curtsk->next);
}
