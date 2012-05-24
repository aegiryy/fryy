#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hack.h"

void FAT12Init() {
    /* Initilize (1) FAT12 header, (2) ROOT sectors and (3) File Allocation Table(FAT). */
    file = fopen("../boot.img", "r+");
    hdr = (FAT12_Hdr *)malloc(sizeof(FAT12_Hdr));
    fseek(file, 0, SEEK_SET);
    fread((void *)hdr, sizeof(FAT12_Hdr), 1, file);

    root = (FAT12_DIR *)malloc(sizeof(FAT12_DIR) * hdr->BPB_RootEntCnt);
    fseek(file, hdr->BPB_BytsPerSec * (hdr->BPB_RsvdSecCnt + hdr->BPB_NumFATs * hdr->BPB_FATSz16), SEEK_SET);
    fread((void *)root, sizeof(FAT12_DIR), hdr->BPB_RootEntCnt, file);

    fat = (unsigned char *)malloc(hdr->BPB_FATSz16 * hdr->BPB_BytsPerSec);
    fseek(file, hdr->BPB_BytsPerSec * hdr->BPB_RsvdSecCnt, SEEK_SET);
    fread((void *)fat, hdr->BPB_FATSz16 * hdr->BPB_BytsPerSec, 1, file);
}

void FAT12DeInit() {
    fclose(file);
    free(hdr);
    free(root);
    free(fat);
}

int FAT12GetFATValue(int index) {
    /* An FAT entry occupies 12 bits (1.5 bytes), this function returns FAT entry value */
    int indbyt = index * 3 / 2;
    int a = fat[indbyt];
    int b = fat[indbyt + 1];
    if (index % 2 == 0)
        return ((b & 0x0f) << 8) | a;
    else
        return (b << 4) | ((a & 0xf0) >> 4);
}

void FAT12PrintFile(FAT12_DIR entry) {
    /* List file specification of an file entry in root sector or dirctories. */
    printf("DIR_NAME: %s\n", entry.name);
    printf("DIR_ATTR: %d\n", entry.attr);
    printf("DIR_WRTTIME: %d\n", entry.wrtTime);
    printf("DIR_FSTCLUS: %d\n", entry.fstClus);
    printf("DIR_FILESIZE: %d\n", entry.fileSize);
}

void FAT12List(char * path) {

}

void list_root() {
    int i = 0;
    for (; i < hdr->BPB_RootEntCnt; i++)
    {
        if (IS_SUBDIR(root[i].attr) || IS_ARCHIVE(root[i].attr))
        {
            FAT12PrintFile(root[i]);
            printf("\n");
        }
    }
}

void print_name(FAT12_DIR entry, int level)
{
    int i;
    char ascname[0xC];
    for (i = 0; i < level; i++)
        printf("\t");
    strncpy(ascname, entry.name, 0xB);
    ascname[0xB] = '\0';
    printf("%s\n", ascname);
}

void list_tree(int level, int index) {
    int i = 0;
    if (level == 0)
    {
        for (i = 0; i < hdr->BPB_RootEntCnt; i++)
        {
            if (IS_SUBDIR(root[i].attr))
            {
                print_name(root[i], level);
                list_tree(level + 1, root[i].fstClus);
            }
            else if (IS_ARCHIVE(root[i].attr))
                print_name(root[i], level);
        }
    }
    else
    {
        do
        {
            FAT12_DIR sector[512 / sizeof(FAT12_DIR)];
            fseek(file, 512 * (index + 31), SEEK_SET);
            fread(sector, 512, 1, file);
            for (i = 0; i < 512 / sizeof(FAT12_DIR); i++)
            {
                if (IS_SUBDIR(sector[i].attr) && strncmp(sector[i].name, ".", 1))
                {
                    print_name(sector[i], level);
                    list_tree(level + 1, sector[i].fstClus);
                }
                else if (IS_ARCHIVE(sector[i].attr))
                    print_name(sector[i], level);
            }
        }
        while ((index = FAT12GetFATValue(index)) < THRESHOLD);
    }
}

void list_secs(int fst) {
    while (FAT12GetFATValue(fst) < THRESHOLD) {
        printf("%d ", fst);
        fst = FAT12GetFATValue(fst);
    }
}

int main(int argc, char * argv[]) {
    FAT12Init();
    list_root();
    FAT12DeInit();
    return 0;
}
