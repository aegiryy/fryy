#include "task.h"

static tcb_t _freelist[MAXTSK];
static tcb_t * _header;
static res_t _reslist[MAXRES];
static int _res_p;
static char _is_init = 0;
static int _count = 0; // task count in running list
static tcb_t * _tail;

static void _init();
static void _add_tcb(tcb_t * tcb);
static tcb_t * _remove_tcb();

tcb_t * task_init(void (*task)(), int cs, int flag)
{
    tcb_t * tcb;
    int i;
    if(!_is_init)
        _init();
    if(_header == (tcb_t *)0)
        /* if not available, return NULL */
        return (tcb_t *)0;
    tcb = _header;
    _header = _header->next;
    for(i = 0; i < REGCNT; i++)
        tcb->reg[i] = 0;
    tcb->reg[IDXIP] = task;
    tcb->reg[IDXFLG] = flag;
    tcb->reg[IDXCS] = cs;
    tcb->reg[IDXDS] = cs;
    tcb->reg[IDXES] = cs;
    tcb->reg[IDXSS] = cs;
    tcb->reg[IDXSP] = &tcb->stk[STKSZ - 1];
    _add_tcb(tcb);
    return tcb;
}

void task_deinit()
{
    ENTER_CRITICAL();
    tcb_t * tcb = _remove_tcb();
    tcb->next = _header->next;
    _header->next = tcb;
    if(curtsk != (tcb_t *)0)
        task_set(curtsk);
    else
    {
        putc('8');
        while(1);
    }
}

void task_set(tcb_t * tcb)
{
    curtsk = tcb;
    asm "mov bx, word [_curtsk]";
    /* switch stack address space */
    asm "mov ss, word 20[bx]";
    asm "mov sp, word 8[bx]";
    /* saving registers */
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
    asm "pop ax";
    asm "iret"; /* implicit STI */
}

int res_init(int c)
{
    ENTER_CRITICAL();
    _reslist[_res_p].count = c;
    _reslist[_res_p].waitlist = (tcb_t *)0;
    EXIT_CRITICAL();
    return _res_p++;
}

void res_p(int res)
{
    ENTER_CRITICAL();
    if(--_reslist[res].count < 0)
    {
        /* remove this task from running list
         * put it in the waitlist of res
         */
        tcb_t * tcb = _remove_tcb();
        if(_reslist[res].waitlist == (tcb_t *)0)
        {
            _reslist[res].waitlist = tcb;
            tcb->next = 0;
        }
        else
        {
            tcb->next = _reslist[res].waitlist->next;
            _reslist[res].waitlist->next = tcb;
        }
        task_set(curtsk);
    }
    EXIT_CRITICAL();
}

void res_v(int res)
{
    ENTER_CRITICAL();
    if(_reslist[res].count++ < 0)
    {
        /* put a task in the waitlist
         * to running list
         */
    }
    EXIT_CRITICAL();
}

/* Below is static functions */

void _init()
{
    int i;
    for(i = 0; i < MAXTSK - 1; i++)
        _freelist[i].next = _freelist + (i + 1);
    _freelist[i].next = (tcb_t *)0;
    _header = _freelist;
    _res_p = 0;
    _is_init = 1;
    curtsk = 0;
}

void _add_tcb(tcb_t * tcb)
{
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
    _count++;
}

tcb_t * _remove_tcb()
{
    tcb_t * tcb = curtsk;
    if(curtsk == 0)
        return 0;
    if(curtsk->next == curtsk)
    {
        curtsk = 0;
    }
    else
    {
        _tail->next = curtsk->next;
        curtsk = curtsk->next;
    }
    _count--;
    return tcb;
}

