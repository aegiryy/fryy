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


tcb_t * task_init(void (*task)(), int cs, int flag);
int task_deinit();
#endif
