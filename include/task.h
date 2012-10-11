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
#include "list.h"

#define TASK_PENDING 0
#define TASK_RUNNING 1
#define TASK_WAITING 2

typedef struct _tcb_t
{
    /* *prev has multi uses like waiting list, 
     * since *next is used for schedule
     */
    LIST_HEAD(struct _tcb_t);
    int ss, sp;
    int state;
    char stk[STKSZ];
} tcb_t;

/* Globally initialize the task system, should be called
 * before anything else
 */
void task_sysinit();
/* Initialize a task, FLAG = 0x0202 */
tcb_t * task_create(void (*task)(), int cs);
/* Set current task */
void task_resume(tcb_t * tcb);
/* Get current task */
tcb_t * task_get();
/* Deinitialize a task, resources would be recycled */
void task_remove(tcb_t * tcb);
/* Interrupt handler, cannot be called */
void task_schedule_irq();
#define task_schedule() asm "sti"; asm "int 0x08"

typedef struct _res_t
{
    LIST_HEAD(struct _res_t);
    int count;
    tcb_t * waitlist;
} res_t;

res_t * res_init(int c);
void res_p(res_t * res);
#define P(res) res_p(res)
void res_v(res_t * res);
#define V(res) res_v(res)

#endif
