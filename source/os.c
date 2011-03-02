#include "os.h"
void putc(char c);
void task1();
void task2();
void task3();
void init_task(tcb_t * tcb, void (*task)(), int cs, int flag, tcb_t * next);
static void set_timer(void (*scheduler)());
static void scheduler();

typedef struct _tcb_t
{
    /* AX BX CX DX
     * SP BP SI DI
     * CS DS SS ES
     * IP FLAGS
     */
    int reg[REGCNT];
    struct _tcb_t * next;
    char stk[STKSZ];
} tcb_t;

tcb_t * curtsk = 0;
tcb_t tcb1;
tcb_t tcb2;
tcb_t tcb3;

void main()
{
    asm "mov ax, cs";
    asm "mov ss, ax";
    init_task(&tcb1, task1, 0x1000, 0x0202, &tcb2);
    init_task(&tcb2, task2, 0x1000, 0x0202, &tcb3);
    init_task(&tcb3, task3, 0x1000, 0x0202, &tcb1);
    set_timer(scheduler);
    curtsk = &tcb1;
    asm "mov bx, word [_curtsk]";
    asm "mov sp, word 8[bx]";
    asm "pushf";
    asm "push word 16[bx]";
    asm "push word 24[bx]";
    asm "iret";
}

void init_task(tcb_t * tcb, void (*task)(), int cs, int flag, tcb_t * next) {
    int i;
    for(i = 0; i < REGCNT; i++)
        tcb->reg[i] = 0;
    tcb->reg[IDXIP] = task;
    tcb->reg[IDXFLG] = flag;
    tcb->reg[IDXCS] = cs;
    tcb->reg[IDXDS] = cs;
    tcb->reg[IDXES] = cs;
    tcb->reg[IDXSS] = cs;
    tcb->reg[IDXSP] = &tcb->stk[STKSZ - 1];
    tcb->next = next;
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

void putc(char c)
{
    c = c;
    asm "push ax";
    asm "push bx";
    asm "mov ah, #0x0e";
    asm "mov al, 4[bp]";
    asm "mov bl, #0x0c";
    asm "int 0x10";
    asm "pop bx";
    asm "pop ax";
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

    asm "mov bx, 28[bx]"; /* bx = tcb->next */
    asm "mov word [_curtsk], bx";

    /* Begin Restoring context of next process */
    /* Switch stack space first */
    asm "mov ss, word 20[bx]";
    asm "mov sp, word 8[bx]";

    asm "push word 26[bx]";
    asm "push word 16[bx]";
    asm "push word 24[bx]";
    asm "push word [bx]";
    asm "push word 2[bx]";
    asm "push word 4[bx]";
    asm "push word 6[bx]";
    asm "push word 10[bx]";
    asm "push word 12[bx]";
    asm "push word 14[bx]";
    asm "push word 18[bx]";
    asm "push word 22[bx]";
    asm "pop es";
    asm "pop ds";
    asm "pop di";
    asm "pop si";
    asm "pop bp";
    asm "pop dx";
    asm "pop cx";
    asm "pop bx";
    asm "mov al, #0x20";
    asm "out #0x20, al";
    asm "pop ax";
    asm "iret";
}
