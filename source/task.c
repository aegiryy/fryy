#include "task.h"

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
    ENTER_CRITICAL();
    if(_count == 1)
    {
        curtsk->next = _header->next;
        _header->next = curtsk;
        curtsk = (tcb_t *)0;
    }
    else
    {
        tcb_t * tcb = curtsk;
        while(tcb->next != curtsk)
            tcb = tcb->next;
        tcb->next = curtsk->next;
        curtsk->next = _header->next;
        _header->next = curtsk;
        curtsk = tcb->next;
    }
    _count--;
    EXIT_CRITICAL();
}

void set_curtsk(tcb_t * tsk)
{
}

/* Below is static functions */

void _init()
{
    int i;
    for(i = 0; i < MAXTSK - 1; i++)
        _freelist[i].next = _freelist + (i + 1);
    _header = _freelist;
    _is_init = 1;
    curtsk = 0;
}
