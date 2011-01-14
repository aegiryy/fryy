void putc(char c);
void task1();
void task2();
void task3();
void init_task(tcb_t * tcb, void (*task)(), int cs, int flag);
static void set_timer(void (*scheduler)());
static void scheduler();

typedef struct _tcb
{
    int ip;
    int cs;
    int flag;
    struct _tcb * next;
} tcb_t;

tcb_t * curtsk = 0;
tcb_t tcb1;
tcb_t tcb2;
tcb_t tcb3;

void main()
{
    init_task(&tcb1, task1, 0x1000, 0x0202, &tcb2);
    init_task(&tcb2, task2, 0x1000, 0x0202, &tcb3);
    init_task(&tcb3, task3, 0x1000, 0x0202, &tcb1);
    set_timer(scheduler);
    asm "jmp _task1";
}

void init_task(tcb_t * tcb, void (*task)(), int cs, int flag, tcb_t * next) {
    tcb->ip = task;
    tcb->cs = cs;
    tcb->flag = flag;
    tcb->next = next;
}

void task1()
{
    while(1)
    {
        asm "push ax";
        asm "push bx";
        asm "mov ah, #0x0e";
        asm "mov al, #65";
        asm "mov bl, #0x0c";
        asm "int 0x10";
        asm "pop bx";
        asm "pop ax";
        // putc('A');
    }
}

void task2()
{
    while(1)
    {
        asm "push ax";
        asm "push bx";
        asm "mov ah, #0x0e";
        asm "mov al, #66";
        asm "mov bl, #0x0c";
        asm "int 0x10";
        asm "pop bx";
        asm "pop ax";
        // putc('B');
    }
}

void task3()
{
    while(1)
    {
        asm "push ax";
        asm "push bx";
        asm "mov ah, #0x0e";
        asm "mov al, #67";
        asm "mov bl, #0x0c";
        asm "int 0x10";
        asm "pop bx";
        asm "pop ax";
        // putc('C');
    }
}

void putc(char c)
{
    c = c;
    asm "push ax";
    asm "push bx";
    asm "mov ah, #0x0e";
    asm "mov al, 4[bp]";
    asm "mov bl, #0x0c";
    asm "int 0x10";
    asm "pop bx";
    asm "pop ax";
}

void set_timer(void (*scheduler)())
{
    scheduler = scheduler;
    asm "push ds";
    asm "mov ax, #0";
    asm "mov ds, ax";
    asm "mov ax, 4[bp]";
    asm "mov word [0x08 * 4], ax";
    asm "mov word [0x08 * 4 + 2], cs";
    asm "pop ds";
}

void scheduler(int cs, int flag)
{
    /* The following commented codes
     * are wrong, for (int *)ptr is 
     * associated with DS rather than
     * SS.
    int * ptr = &flag;
    curtsk->flag = *ptr;
    ptr--;
    curtsk->cs = *ptr;
    ptr--;
    curtsk->ip = *ptr;
    curtsk = curtsk->next;
    *ptr = curtsk->ip;
    ptr++;
    *ptr = curtsk->cs;
    ptr++;
    *ptr = curtsk->flag;
    */
    asm "push ax";
    asm "push bx";

    asm "add sp, #4";
    asm "mov bx, word [_curtsk]";
    asm "pop word [bx]";
    asm "pop word 2[bx]";
    asm "pop word 4[bx]";
    asm "mov bx, 6[bx]";
    asm "mov word [_curtsk], bx";
    asm "push word 4[bx]";
    asm "push word 2[bx]";
    asm "push word [bx]";
    asm "sub sp, #4";

    asm "mov al, #0x20";
    asm "out #0x20, al";
    asm "pop bx";
    asm "pop ax";
    asm "iret";
}
