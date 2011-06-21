#ifndef _FAT12_H
#define _FAT12_H

#define THRESHOLD 0xff8
#define SECTOR_SIZE 512
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

#define PHYSICAL_SECTOR(fstClus) (31+(fstClus))
#define IS_FREE(entry) ((entry).name[0] == 0xE5 || (entry).name[0] == 0x00)
#define IS_ROOT(entry) ((entry).fstClus == 0)
#define ATTR_READONLY(entry) ((entry).attr & 0x01)
#define ATTR_HIDDEN(entry) ((entry).attr & 0x02)
#define ATTR_SYSTEM(entry) ((entry).attr & 0x04)
#define ATTR_DIRECTORY(entry) ((entry).attr & 0x10)
#define ATTR_ARCHIVE(entry) ((entry).attr & 0x20)
int fat_value(int fstClus);

#endif
