#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H
typedef struct _file_t
{
    char name[11];
    char attr;
    int fstCluster;
    int filesize[2];
} file_t;

#endif
