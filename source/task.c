#include "task.h"
#include "io.h"

static tcb_t _freelist[MAXTSK];
static tcb_t * _header;
static tcb_t * _curtsk; /* current task */
static res_t _reslist[MAXRES];
static void task_stub();
static int _res_p;

tcb_t * task_create(void (*task)(), int cs)
{
    tcb_t * tcb;
    if(_header == (tcb_t *)0)
        /* if not available, return NULL */
        return (tcb_t *)0;
    tcb = _header;
    _header = _header->prev;
    tcb->ss = KERNELBASE;
    tcb->sp = &tcb->stk[STKSZ-24];
    *(int *)&tcb->stk[STKSZ-2*(IDXFLG+1)] = (int)0x0282;
    *(int *)&tcb->stk[STKSZ-2*(IDXCS+1)] = (int)cs;
    *(int *)&tcb->stk[STKSZ-2*(IDXIP+1)] = (int)task;
    /* don't forget to setup the DS */
    *(int *)&tcb->stk[STKSZ-2*(IDXDS+1)] = (int)cs;
    tcb->state = TASK_PENDING;
    CDLIST_ADD(_curtsk, tcb);
    return tcb;
}

/* Remove a task. If no task is running, system
 * will go shutdown
 */
void task_remove(tcb_t * tcb)
{
    ENTER_CRITICAL();
    if(tcb->tid == 0)
    {
        puts("Now you can halt your machine.");
        while(1);
    }
    tcb->state = TASK_ZOMBIE;
    CDLIST_REMOVE(tcb);
    LSLIST_ADD(_header, tcb);
    task_schedule();
    EXIT_CRITICAL();
}

void task_resume(tcb_t * tcb)
{
    ENTER_CRITICAL();
    _curtsk = tcb;
    _curtsk->state = TASK_RUNNING;
    asm "mov bx, word [__curtsk]";
    /* switch stack address space */
    asm "mov ss, word 4[bx]";
    asm "mov sp, word 6[bx]";
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

tcb_t * task_get() {
    return _curtsk;
}

/* Before call task schedule, FLAG, CS, IP should
 * be pushed into stack */
void task_schedule_irq()
{
    asm "push ax";
    asm "push bx";
    asm "push cx";
    asm "push dx";
    asm "push bp";
    asm "push si";
    asm "push di";
    asm "push ds";
    asm "push es";
    // get current task 
    asm "mov bx, word [__curtsk]";
    asm "mov word 4[bx], ss";
    asm "mov word 6[bx], sp";
    // context switch
    asm "mov al, #0x20";
    asm "out #0x20, al";
    task_resume(_curtsk->next);
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
    /* Save task context for potential schedule */
    ENTER_CRITICAL();
    if(--(res->count) < 0)
    {
        /* remove this task from running list
         * put it in the waitlist of res
         */
        CDLIST_REMOVE(_curtsk);
        _curtsk->state = TASK_WAITING;
        LSLIST_ADD(res->waitlist, _curtsk);
        task_schedule();
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
        res->waitlist = res->waitlist->prev;
        CDLIST_ADD(_curtsk, tcb);
    }
    EXIT_CRITICAL();
}

/* Init freelist and related global (also static) variables */
void task_sysinit()
{
    int i;
    for(i = 0; i < MAXTSK - 1; i++)
    {
        _freelist[i].prev = _freelist + (i + 1);
        _freelist[i].state = TASK_ZOMBIE;
        _freelist[i].tid = i;
    }
    _freelist[i].prev = (tcb_t *)0; /* make the list a circle */
    _freelist[i].tid = i;
    _header = _freelist;
    _res_p = 0;
    _curtsk = 0;
    task_create(task_stub, KERNELBASE);
}

void task_stub()
{
    while(1)
        task_schedule();
}
