#include "task.h"
#include "io.h"

static tcb_t _freelist[MAXTSK];
static tcb_t * _header;
static int _count = 0; // task count in running list
static tcb_t * _curtsk; /* current task */
static res_t _reslist[MAXRES];
static int _res_p;

static void _add_tcb(tcb_t * tcb);
static void _remove_tcb(tcb_t * tcb);


tcb_t * task_create(void (*task)(), int cs)
{
    tcb_t * tcb;
    if(_header == (tcb_t *)0)
        /* if not available, return NULL */
        return (tcb_t *)0;
    tcb = _header;
    _header = _header->next;
    tcb->ss = KERNELBASE;
    tcb->sp = &tcb->stk[STKSZ-24];
    *(int *)&tcb->stk[STKSZ-2] = (int)0x0282;
    *(int *)&tcb->stk[STKSZ-4] = (int)cs;
    *(int *)&tcb->stk[STKSZ-6] = (int)task;
    /* don't forget setup the DS */
    *(int *)&tcb->stk[STKSZ-22] = (int)cs;
    tcb->state = TASK_PENDING;
    _add_tcb(tcb);
    return tcb;
}

void task_remove(tcb_t * tcb)
{
    asm "pop ax";
    asm "pushf"; /* INTR should be open */
    asm "push cs";
    asm "push ax";
    asm "call _task_save";
    asm "mov bx, word [__curtsk]"; /* push RETURN ADDRESS */
    asm "push word 24[bx]";
    _remove_tcb(tcb);
    tcb->next = _header->next;
    _header->next = tcb;
    if(_curtsk != (tcb_t *)0)
    {
        task_resume(_curtsk);
    }
    else
    {
        puts("*#END#*");
        while(1);
    }
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
    asm "push si";
    asm "push di";
    asm "push ds";
    asm "push es";
    // get current task 
    asm "mov bx, word [__curtsk]";
    asm "mov word 4[bx], ss";
    asm "mov word 6[bx], sp";
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
    asm "pop ax";
    asm "pushf"; /* INTR should be open */
    asm "push cs";
    asm "push ax";
    ENTER_CRITICAL();
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
        task_resume(_curtsk);
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

/* Init freelist and related global (also static) variables */
void task_sysinit()
{
    int i;
    for(i = 0; i < MAXTSK - 1; i++)
        _freelist[i].next = _freelist + (i + 1);
    _freelist[i].next = (tcb_t *)0; /* make the list a circle */
    _header = _freelist;
    _res_p = 0;
    _curtsk = 0;
}

/* Below is static functions */

/* add a tcb to RUNNING tasks */
void _add_tcb(tcb_t * tcb)
{
    LIST_ADD(_curtsk, tcb);
    _count++;
}

/* remove current tcb from RUNNING tasks */
void _remove_tcb(tcb_t * tcb)
{
    if (tcb == 0)
        return;
    LIST_REMOVE(tcb);
    if (_curtsk == tcb && _curtsk != 0)
        _curtsk = _curtsk->next;
    _count--;
}
