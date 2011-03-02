#include "task.h"

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
