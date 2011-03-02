#include "task.h"

tcb_t freelist[MAXTSK];
static char is_init = 0;
static void global_init();
static tcb_t * header;
static int count = 0;

void task_init(tcb_t * tcb, void (*task)(), int cs, int flag, tcb_t * next) {
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

tcb_t * task_alloc(void (*task)(), int cs, int flag)
{
    tcb_t * tcb;
    int i;
    if(!is_init)
        global_init();
    if(count >= MAXTSK)
        return (tcb_t *)0;
    for(i = 0; i < REGCNT; i++)
        tcb->reg[i] = 0;
    //count++;
    tcb->reg[IDXIP] = task;
    tcb->reg[IDXFLG] = flag;
    tcb->reg[IDXCS] = cs;
    tcb->reg[IDXDS] = cs;
    tcb->reg[IDXES] = cs;
    tcb->reg[IDXSS] = cs;
    tcb->reg[IDXSP] = &tcb->stk[STKSZ - 1];
    return tcb;
}

/* Below is static functions */

void global_init()
{
    int i;
    for(i = 0; i < MAXTSK - 1; i++)
        freelist[i].next = freelist + (i + 1);
    header = freelist;
    is_init = 1;
}
