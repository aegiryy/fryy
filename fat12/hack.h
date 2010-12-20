#define SECT(fstClus) ((fstClus) - 2 + hdr->BPB_RsvdSecCnt + hdr->BPB_FATSz16 * hdr->BPB_NumFATs + hdr->BPB_RootEntCnt * sizeof(FAT12_DIR) / hdr->BPB_BytsPerSec)
#define THRESHOLD 0xff8

/* pack(n) will force compiler do n-byte align */
#pragma pack(1)
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
    unsigned short int BPB_RsvdSecCnt;
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
#pragma pack()

FILE * file;
FAT12_Hdr * hdr;
FAT12_DIR * root;
unsigned char * fat;
