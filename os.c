#include "os.h"
void putc(char c);
void task1();
void task2();
void task3();
void init_task(pcb_t * pcb, void (*task)(), int cs, int flag, pcb_t * next);
static void set_timer(void (*scheduler)());
static void scheduler();

typedef struct _pcb_t
{
    /* AX BX CX DX
     * SP BP SI DI
     * CS DS SS ES
     * IP FLAGS
     */
    int reg[REGCNT];
    struct _pcb_t * next;
    char stk[STKSZ];
} pcb_t;

pcb_t * curtsk = 0;
pcb_t pcb1;
pcb_t pcb2;
pcb_t pcb3;

void main()
{
    asm "mov ax, cs";
    asm "mov ss, ax";
    init_task(&pcb1, task1, 0x1000, 0x0202, &pcb2);
    init_task(&pcb2, task2, 0x1000, 0x0202, &pcb3);
    init_task(&pcb3, task3, 0x1000, 0x0202, &pcb1);
    set_timer(scheduler);
    curtsk = &pcb1;
    asm "mov bx, word [_curtsk]";
    asm "mov sp, word 8[bx]";
    asm "pushf";
    asm "push word 16[bx]";
    asm "push word 24[bx]";
    asm "iret";
}

void init_task(pcb_t * pcb, void (*task)(), int cs, int flag, pcb_t * next) {
    int i;
    for(i = 0; i < REGCNT; i++)
        pcb->reg[i] = 0;
    pcb->reg[IDXIP] = task;
    pcb->reg[IDXFLG] = flag;
    pcb->reg[IDXCS] = cs;
    pcb->reg[IDXDS] = cs;
    pcb->reg[IDXES] = cs;
    pcb->reg[IDXSS] = cs;
    pcb->reg[IDXSP] = &pcb->stk[STKSZ - 1];
    pcb->next = next;
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

    asm "mov bx, 28[bx]"; /* bx = pcb->next */
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
