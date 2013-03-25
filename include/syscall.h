#ifndef _SYSCALL_H
#define _SYSCALL_H

/* System Call should be called like the following
 * push param3; params passing from right to left
 * push param2;
 * push param1;
 * int 0x21;
 * mov local_var, ax; AX = return value
 */

#include "task.h"

#define N_task_remove 0
#define N_task_get 1

void syscall_sysinit();

extern void *syscall_table[];
#endif
