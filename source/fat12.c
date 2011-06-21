#include "fat12.h"
#include "io.h"

char sector[SECTOR_SIZE];
static char fat[SECTOR_SIZE];

int fat_walkthrough(fat_entry_t * dir, int (*handler)(fat_entry_t * entry))
{
    int i, j;
    if (IS_ROOT(dir))
    {
        for (i = 19; i < 33; i++)
        {
            load_sectors(sector, i, 1);
            for (j = 0; j < SECTOR_SIZE / sizeof(fat_entry_t); j++)
                if (handler(((fat_entry_t *)sector)+j))
                    return 1;
        }
    }
    else
    {
        int clus = dir->fstClus;
        do
        {
            load_sectors(sector, PHYSICAL_SECTOR(clus), 1);
            clus = fat_value(clus);
            for (j = 0; j < SECTOR_SIZE / sizeof(fat_entry_t); j++)
                if (handler(((fat_entry_t *)sector)+j))
                    return 1;
        } while (clus < THRESHOLD);
    }
    return 0;
}

int fat_value(int fstClus)
{
    int byteidx = fstClus * 3 / 2;
    int a, b;
    load_sectors(fat, byteidx / SECTOR_SIZE + 1, 1);
    byteidx = byteidx % SECTOR_SIZE;
    a = fat[byteidx];
    b = fat[byteidx + 1];
    if (fstClus % 2 == 0)
        return ((b & 0x0f) << 8) | a;
    else
        return (b << 4) | ((a & 0xf0) >> 4);
}
