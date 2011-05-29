#include "io.h"

static int _mod(int a, int b);

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

void puts(char * s)
{
    while(*s)
    {
        putc(*s);
        s++;
    }
}

void print(int n)
{
    char bit[10];
    char p = 0;
    while (n > 0)
    {
        bit[p++] = _mod(n, 10);
        n /= 10;
    }
    while (p--)
    {
        putc('0' + bit[p]);
    }
}

static int _mod(int a, int b)
{
    int c = a / b;
    return a - b * c;
}
