#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hack.h"

void init() {
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

int FAT12GetFATValue(int index) {
    int indbyt = index * 3 / 2;
    int a = fat[indbyt];
    int b = fat[indbyt + 1];
    if (index % 2 == 0)
        return ((b & 0x0f) << 8) | a;
    else
        return (b << 4) | ((a & 0xf0) >> 4);
}

void FAT12PrintFile(FAT12_DIR entry) {
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
        if (root[i].fstClus != 0 && FAT12GetFATValue(root[i].fstClus) != 0 && root[i].name[0] != 229) {
            FAT12PrintFile(root[i]);
            printf("\n");
        }
}

void list_secs(int fst) {
    while (FAT12GetFATValue(fst) < THRESHOLD) {
        printf("%d ", fst);
        fst = FAT12GetFATValue(fst);
    }
}

int main(int argc, char * argv[]) {
    int i, s = 0;
    FAT12_DIR inh[16];
    init();
    list_root();
    /*
    for (i = 0; i < hdr->BPB_RootEntCnt; i++)
        if (root[i].fstClus != 0 && FAT12GetFATValue(root[i].fstClus) != 0 && root[i].attr == 16) {
            fseek(file, SECT(root[i].fstClus) * 512, SEEK_SET);
            fread((void *)inh, sizeof(FAT12_DIR), 16, file);
            break;
        }
    for (i = 0; i < 16; i++) {
        FAT12PrintFile(inh[i]);
        printf("\n");
    }
    */
    return 0;
}
