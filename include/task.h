/* FEATURES:
 *      Task initialization
 *      Task ending
 *      Set current task
 *      Resource management
 */

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

/* Initialize a task, FLAG = 0x0202 */
tcb_t * task_init(void (*task)(), int cs);
/* Set current task */
void task_set(tcb_t * tcb);
/* Get current task */
tcb_t * task_get();
/* Deinitialize a task, resources would be recycled */
void task_deinit(tcb_t * tcb);
/* Core scheduler */
void task_schedule();

typedef struct _res_t
{
    int count;
    tcb_t * waitlist;
    struct _res_t * next;
} res_t;

int res_init(int c);
void res_p(int res);
#define P(res) res_p(res)
void res_v(int res);
#define V(res) res_v(res)

#endif
