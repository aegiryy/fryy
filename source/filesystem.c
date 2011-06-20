#include "filesystem.h"
#include "io.h"
#include "fat12.h"
#include <string.h>

char content[512];

void fsdriver()
{
    int i;
    fat_entry_t * entry;
    load_sectors(content, 19, 1);
    entry = (fat_entry_t *)content;
    for (i = 0; i < 16; i++)
        if (strncmp("OS      BIN", entry[i].name, 11) == 0)
        {
            puts("OS.BIN Found!");
            ENTER();
            list_secs(entry[i].fstClus);
            break;
        }
}

