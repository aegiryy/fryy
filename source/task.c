#include "task.h"

extern tcb_t * curtsk = 0;

static tcb_t _freelist[MAXTSK];
static char _is_init = 0;
static void _init();
static tcb_t * _header;
static int _count = 0;
static tcb_t * _tail;

tcb_t * task_init(void (*task)(), int cs, int flag)
{
    tcb_t * tcb;
    int i;
    if(!_is_init)
        _init();
    if(_count == MAXTSK)
        /* if not available, return NULL */
        return (tcb_t *)0;
    tcb = _header;
    _header = _header->next;
    for(i = 0; i < REGCNT; i++)
        tcb->reg[i] = 0;
    _count++;
    tcb->reg[IDXIP] = task;
    tcb->reg[IDXFLG] = flag;
    tcb->reg[IDXCS] = cs;
    tcb->reg[IDXDS] = cs;
    tcb->reg[IDXES] = cs;
    tcb->reg[IDXSS] = cs;
    tcb->reg[IDXSP] = &tcb->stk[STKSZ - 1];
    if(curtsk == 0)
    {
        curtsk = tcb;
        tcb->next = curtsk;
        _tail = curtsk;
    }
    else
    {
        _tail->next = tcb;
        _tail = _tail->next;
        _tail->next = curtsk;
    }
    return tcb;
}

int task_deinit()
{
    curtsk->next = _header->next;
    _header->next = curtsk;
    _count++;
}

/* Below is static functions */

void _init()
{
    int i;
    for(i = 0; i < MAXTSK - 1; i++)
        _freelist[i].next = _freelist + (i + 1);
    _header = _freelist;
    _is_init = 1;
}
