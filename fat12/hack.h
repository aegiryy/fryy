#define ROOT_SEC_NUM 14
#define SEC_SIZE 512

#define SECT(fstClus) (((fstClus) - 2) + 19 + ROOT_SEC_NUM)

typedef struct {
    unsigned char name[0xB];
    unsigned char attr;
    unsigned char resv[10];
    unsigned short int wrtTime;
    unsigned short int wrtDate;
    unsigned short int fstClus;
    unsigned int fileSize;
} FAT12_DIR;

typedef struct {
    unsigned char BS_jmpBoot[3];
    unsigned char BS_OEMName[8];
    unsigned short int BPB_BytsPerSec;
    unsigned char BPB_SecPerClus;
    unsigned short BPB_RsvdSecCnt;
    unsigned char BPB_NumFATs;
    unsigned short int BPB_RootEntCnt;
    unsigned short int BPB_TotSec16;
    unsigned char BPB_Media;
    unsigned short int BPB_FATSz16;
    unsigned short int BPB_SecPerTrk;
    unsigned short int BPB_NumHeads;
    unsigned int BPB_HiddSec;
    unsigned int BPB_TotSec32;
    unsigned char BS_DrvNum;
    unsigned char BS_Reserved1;
    unsigned char BS_BootSig;
    unsigned int BS_VolID;
    unsigned char BS_VolLab[11];
    unsigned char BS_FileSysType[8];
} FAT12_Hdr;

FILE * file;
