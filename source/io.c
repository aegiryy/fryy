#include "io.h"

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

char getc()
{
    char c = 'x';
    asm "mov ah, #0";
    asm "int 0x16";
    asm "mov -5[bp], al";
    return c;
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
    if (n == 0)
    {
        putc('0');
        return;
    }
    while (n > 0)
    {
        bit[p++] = n % 10;
        n /= 10;
    }
    while (p--)
    {
        putc('0' + bit[p]);
    }
}

void load_sectors(char * addr, int sector_no, char count)
{
    sector_no = sector_no;
    asm "mov ax, cs";
    asm "mov es, ax";
    asm "mov bx, 4[bp]";
    asm "push bx";
    asm "mov ax, 6[bp]";
    asm "mov bl, #18";
    asm "div bl";
    asm "inc ah";
    asm "mov cl, ah";
    asm "mov ch, al";
    asm "shr ch, #1";
    asm "mov dh, al";
    asm "and dh, #1";
    asm "mov dl, #0"; /* Drive Number */
    asm "pop bx";
    asm "_load_sectors_redo: mov ah, #2";
    asm "mov al, 8[bp]";
    asm "int 0x13";
    asm "jc _load_sectors_redo";
}
