#ifndef _IO_H
#define _IO_H

void putc(char c);
void puts(char * s);
void print(int n);
#define ENTER() putc(13);putc(10)

void load_sectors(char * addr, int sector_no, char count);
#endif
