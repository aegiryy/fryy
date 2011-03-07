#ifndef _TASK_H
#define _TASK_H

#include "config.h"
#include "cpu.h"

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

typedef struct _res_t
{
    int count;
    tcb_t * waitlist;
} res_t;

tcb_t * curtsk; /* current task */

tcb_t * task_init(void (*task)(), int cs, int flag);
void task_set(tcb_t * tcb);
void task_deinit();
int res_init(int c);
#endif
