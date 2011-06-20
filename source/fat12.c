#include "fat12.h"
#include "io.h"
#define THRESHOLD 0xff8

static char fat[512];

int fat_value(int fstClus)
{
    int byteidx = fstClus * 3 / 2;
    int a, b;
    load_sectors(fat, byteidx / 512 + 1, 1);
    byteidx = byteidx % 512;
    a = fat[byteidx];
    b = fat[byteidx + 1];
    if (fstClus % 2 == 0)
        return ((b & 0x0f) << 8) | a;
    else
        return (b << 4) | ((a & 0xf0) >> 4);
}

void list_secs(int fstClus)
{
    while (fat_value(fstClus) < THRESHOLD)
    {
        print(fstClus);
        ENTER();
        fstClus = fat_value(fstClus);
    }
}
