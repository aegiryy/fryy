#include <stdio.h>
#include "hack.h"

int main(int argc, char * argv[]) {
    /*
    file = fopen("../boot", "r");
    FAT12_Hdr hdr;
    fread((void *)&hdr, sizeof(FAT12_Hdr), 1, file);
    printf("%s\n", hdr.BS_OEMName);
    */
    printf("%d\n", (int)sizeof(short int));
    return 0;
}
