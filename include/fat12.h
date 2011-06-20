#ifndef _FAT12_H
#define _FAT12_H

#pragma pack(1)
typedef struct {
    char name[0xB];
    char attr;
    char resv[10];
    int wrtTime;
    int wrtDate;
    int fstClus;
    int filesize[2];
} fat_entry_t;
#pragma pack()

#define PHYSICAL_SECTOR(fstClus) ((fstClus) == 0? 19: (31+(fstClus)))
int fat_value(int fstClus);
void list_secs(int fstClus);

#endif
