#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hack.h"

void init() {
    file = fopen("../boot", "r+");
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

int FAT12GetFATValue(int index) {
    int indbyt = index * 3 / 2;
    if (index % 2)
        return ((fat[indbyt] & 0x0f) << 8) | fat[indbyt + 1];
    else
        return ((fat[indbyt] << 8) | (fat[indbyt + 1] & 0xf0)) >> 4;
}

void FAT12PrintFile(FAT12_DIR entry) {
    printf("DIR_NAME: %s\n", entry.name);
    printf("DIR_ATTR: %d\n", entry.attr);
    printf("DIR_WRTTIME: %d\n", entry.wrtTime);
    printf("DIR_FSTCLUS: %d\n", entry.fstClus);
    printf("DIR_FILESIZE: %d\n", entry.fileSize);
}

int main(int argc, char * argv[]) {
    int i, s = 0;
    char buffer[100];
    init();
    for (i = 0; i < hdr->BPB_RootEntCnt; i++)
        if (root[i].fstClus != 0 && FAT12GetFATValue(root[i].fstClus) != 0) {
            FAT12PrintFile(root[i]);
            s++;
        }
    printf("sum: %d files\n", s);
    printf("%d\n", FAT12GetFATValue(3));
    /*
    fseek(file, SECT(11) * 512, SEEK_SET);
    fread((void *)buffer, 4, 1, file);
    buffer[4] = 0;
    printf("%s\n", buffer);
    */
    return 0;
}
