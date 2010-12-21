jmp short START
nop

; FAT12 header
BS_OEMName      db  "FRYY0.1", 0
BPB_BytsPerSec  dw  0x200
BPB_SecPerClus  db  0x1
BPB_RsvdSecCnt  dw  0x1 ; Boot Sector Count
BPB_NumFATs     db  0x2
BPB_RootEntCnt  dw  0xe0
BPB_TotSec16    dw  0xb40
BPB_Media       db  0xf0
BPB_FATSz16     dw  0x9
BPB_SecPerTrk   dw  0x12
BPB_NumHeads    dw  0x2
BPB_HiddSec     dd  0x0
BPB_TotSec32    dd  0x0
BS_DrvNum       db  0x0
BS_Reserved1    db  0x0
BS_BootSig      db  0x29
BS_VolID        dd  0x0
BS_VolLab       db  "NO NAME   ", 0
BS_FileSysType  db  "FAT12  ", 0   

START:

times 510-($-$$) db 0
dw 0xaa55
db 0xf0, 0xff, 0xff
times 1024-($-$$) db 0
