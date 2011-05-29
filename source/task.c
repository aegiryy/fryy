#include "task.h"
#include "io.h"

static tcb_t _freelist[MAXTSK];
static tcb_t * _header;
static char _is_init = 0;
static int _count = 0; // task count in running list
static tcb_t * _curtsk; /* current task */
static res_t _reslist[MAXRES];
static int _res_p;

static void _init();
static void _add_tcb(tcb_t * tcb);
static void _remove_tcb(tcb_t * tcb);
static tcb_t * _get_tail(tcb_t * start);


tcb_t * task_init(void (*task)(), int cs)
{
    tcb_t * tcb;
    int i, flag = 0x0202;
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

void task_deinit(tcb_t * tcb)
{
    ENTER_CRITICAL();
    int is_curtsk = (tcb == _curtsk);
    _remove_tcb(tcb);
    tcb->next = _header->next;
    _header->next = tcb;
    if(_curtsk != (tcb_t *)0)
    {
        if (is_curtsk)
            task_set(_curtsk);
        else
        {
            EXIT_CRITICAL();
            return;
        }
    }
    else
    {
        puts("*#END#*");
        while(1);
    }
}

void task_set(tcb_t * tcb)
{
    ENTER_CRITICAL();
    _curtsk = tcb;
    asm "mov bx, word [__curtsk]";
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

/* must be called after pushing FLAG, CS, IP
 * only AX, BX would be affected
 */
void task_save() {
    ENTER_CRITICAL();
    asm "push ax";
    asm "push bx";
    asm "push cx";
    asm "push dx";
    asm "push bp";
    // SP should be saved seperatly 
    asm "push si";
    asm "push di";
    asm "push ds";
    asm "push ss";
    asm "push es";
    // get current task 
    asm "mov bx, word [__curtsk]";
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
    asm "pop ax"; // return address
    asm "pop word 24[bx]";
    asm "pop word 16[bx]";
    asm "pop word 26[bx]";
    asm "mov word 8[bx], sp";

    asm "push ax";
    EXIT_CRITICAL();
}

tcb_t * task_get() {
    return _curtsk;
}

/* Before call task schedule, FLAG, CS, IP should
 * be pushed into stack */
void task_schedule()
{
    asm "call _task_save";
    asm "mov al, #0x20";
    asm "out #0x20, al";
    task_set(_curtsk->next);
}

res_t * res_init(int c)
{
    ENTER_CRITICAL();
    _reslist[_res_p].count = c;
    _reslist[_res_p].waitlist = (tcb_t *)0;
    EXIT_CRITICAL();
    return &_reslist[_res_p++];
}

void res_p(res_t * res)
{
    ENTER_CRITICAL();
    /* Save task context for potential schedule */
    asm "pop ax";
    asm "pushf";
    asm "push cs";
    asm "push ax";
    asm "call _task_save";
    asm "mov bx, word [__curtsk]";
    asm "push word 24[bx]";
    if(--(res->count) < 0)
    {
        /* remove this task from running list
         * put it in the waitlist of res
         */
        tcb_t * tcb = _curtsk;
        _remove_tcb(tcb);
        if(res->waitlist == (tcb_t *)0)
        {
            res->waitlist = tcb;
            tcb->next = 0;
        }
        else
        {
            tcb->next = res->waitlist->next;
            res->waitlist->next = tcb;
        }
        task_set(_curtsk);
    }
    EXIT_CRITICAL();
}

void res_v(res_t * res)
{
    ENTER_CRITICAL();
    if(res->count++ < 0)
    {
        /* put a task in the waitlist
         * to running list
         */
        tcb_t * tcb = res->waitlist;
        res->waitlist = res->waitlist->next;
        _add_tcb(tcb);
    }
    EXIT_CRITICAL();
}

/* Below is static functions */

/* Init freelist and related global (also static) variables */
void _init()
{
    int i;
    for(i = 0; i < MAXTSK - 1; i++)
        _freelist[i].next = _freelist + (i + 1);
    _freelist[i].next = (tcb_t *)0; /* make the list a circle */
    _header = _freelist;
    _res_p = 0;
    _is_init = 1;
    _curtsk = 0;
}

/* add a tcb to RUNNING tasks */
void _add_tcb(tcb_t * tcb)
{
    if(_curtsk == 0)
    {
        _curtsk = tcb;
        tcb->next = _curtsk;
    }
    else
    {
        tcb_t * tail = _get_tail(_curtsk);
        tail->next = tcb;
        tail = tail->next;
        tail->next = _curtsk;
    }
    _count++;
}

/* remove current tcb from RUNNING tasks */
void _remove_tcb(tcb_t * tcb)
{
    if(_curtsk == 0)
        return;
    if(_curtsk->next == _curtsk)
        _curtsk = 0;
    else
    {
        tcb_t * tail = _get_tail(tcb);
        tail->next = tcb->next;
        if (_curtsk == tcb)
            _curtsk = _curtsk->next;
    }
    _count--;
}

/* return tail tcb of a list started by start */
tcb_t * _get_tail(tcb_t * start)
{
    tcb_t * tcb = start;
    if(start == 0)
        return 0;
    while(tcb->next != start)
        tcb = tcb->next;
    return tcb;
}
