#include "kernel.h"

void main()
{
    asm "mov ax, cs";
    asm "mov ss, ax";
    asm "mov sp, #0";
    syscall_sysinit();
    task_sysinit();
    task_create(shell, KERNELBASE);
    task_resume(task_get());
}
