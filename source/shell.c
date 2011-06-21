#include "shell.h"
#define BEGIN_CMD() int task_func = _task_func
#define END_CMD() if (task_func == 0) task_deinit(task_get()); else return
#define PARAMETER(cmd_length) (buffer[(cmd_length)+1])

static int _task_func = 0;
static fat_entry_t cd;
static char buffer[BUFSZ];
static void cmd_echo();
static void cmd_exit();
static void cmd_cd();
static void cmd_dir();
static void cmd_cat();
static void (*find_procedure(char * cmd))();
static char sector[SECTOR_SIZE];
static fat_entry_t * lookup_fat_entry(fat_entry_t * entry);
static void display_fat_entry(fat_entry_t * entry);
static void catfile(fat_entry_t * entry);

void shell()
{
    char c;
    cd.fstClus = 0;
    while (1)
    {
        int p = 0;
        void (*f)();
        puts("$> ");
        while (1)
        {
            c = getc();
            if (c == 13)
            {
                buffer[p++] = '\0';
                ENTER();
                break;
            }
            else if (c == 8)
            {
                if (p > 0)
                {
                    p--;
                    putc(c);
                    putc(' ');
                    putc(c);
                }
            }
            else
            {
                putc(c);
                buffer[p++] = c;
            }
        }
        if ((f = find_procedure(buffer)) == 0)
        {
            puts("command not found!");
            ENTER();
            continue;
        }
        else
        {
            if (buffer[p-2] == '&')
            {
                p -= 2;
                buffer[p] = '\0';
                _task_func = 0;
                task_init(f, 0x1000);
            }
            else
            {
                _task_func = 1;
                f();
            }
        }
    }
}
static void (*find_procedure(char * cmd))()
{
    if (strncmp("echo", buffer, 4) == 0)
        return cmd_echo;
    if (strncmp("exit", buffer, 4) == 0)
        return cmd_exit;
    if (strncmp("dir", buffer, 3) == 0)
        return cmd_dir;
    if (strncmp("cd", buffer, 2) == 0)
        return cmd_cd;
    if (strncmp("cat", buffer, 3) == 0)
        return cmd_cat;
    return 0;
}

static void cmd_echo()
{
    int i;
    BEGIN_CMD();
    for(i = 5; buffer[i] != '\0'; i++)
        putc(buffer[i]);
    ENTER();
    END_CMD();
}

static void cmd_exit()
{
    task_deinit(task_get());
}

static void cmd_dir()
{
    int i;
    int j;
    BEGIN_CMD();
    if (IS_ROOT(cd))
    {
        for (i = 19; i < 33; i++)
        {
            fat_entry_t * ent;
            load_sectors(sector, i, 1);
            display_fat_entry(sector);
        }
    }
    else
    {
        int clus = cd.fstClus;
        do
        {
            int i;
            fat_entry_t * ent;
            load_sectors(sector, PHYSICAL_SECTOR(clus), 1);
            clus = fat_value(clus);
            display_fat_entry(sector);
        } while (clus < THRESHOLD);
    }
    END_CMD();
}

static void cmd_cd()
{
    int i;
    BEGIN_CMD();
    if (IS_ROOT(cd))
    {
        for (i = 19; i < 33; i++)
        {
            fat_entry_t * ent;
            load_sectors(sector, i, 1);
            if (ent = lookup_fat_entry(sector, buffer+3))
            {
                cd = *ent;
                END_CMD();
            }
        }
    }
    else
    {
        int clus = cd.fstClus;
        do
        {
            fat_entry_t * ent;
            load_sectors(sector, PHYSICAL_SECTOR(clus), 1);
            clus = fat_value(clus);
            if (ent = lookup_fat_entry(sector, buffer+3))
            {
                cd = *ent;
                END_CMD();
            }
        } while (clus < THRESHOLD);
    }
    puts("No such directory!");
    ENTER();
    END_CMD();
}

static void cmd_cat()
{
    int i;
    BEGIN_CMD();
    if (IS_ROOT(cd))
    {
        for (i = 19; i < 33; i++)
        {
            fat_entry_t * ent;
            load_sectors(sector, i, 1);
            if (ent = lookup_fat_entry(sector, buffer+4))
            {
                catfile(ent);
                ENTER();
                END_CMD();
            }
        }
    }
    else
    {
        int clus = cd.fstClus;
        do
        {
            fat_entry_t * ent;
            load_sectors(sector, PHYSICAL_SECTOR(clus), 1);
            clus = fat_value(clus);
            if (ent = lookup_fat_entry(sector, buffer+4))
            {
                catfile(ent);
                ENTER();
                END_CMD();
            }
        } while (clus < THRESHOLD);
    }
    puts("No such file!");
    ENTER();
    END_CMD();

}

static fat_entry_t * lookup_fat_entry(fat_entry_t * ent, char * filename)
{
    int j;
    for (j = 0; j < SECTOR_SIZE / sizeof(fat_entry_t); j++)
    {
        if (IS_FREE(ent[j]))
            continue;
        if (ATTR_DIRECTORY(ent[j]) || ATTR_ARCHIVE(ent[j]))
            if (strncmp(ent[j].name, filename, strlen(filename)) == 0)
                return ent + j;
    }
    return 0;
}

static void display_fat_entry(fat_entry_t * ent)
{
    int i;
    for (i = 0; i < SECTOR_SIZE / sizeof(fat_entry_t); i++)
    {
        ent[i].attr &= 0x7f;
        if (IS_FREE(ent[i]))
            continue;
        if (ATTR_DIRECTORY(ent[i]))
        {
            puts(ent[i].name);
            ENTER();
        }
        if (ATTR_ARCHIVE(ent[i]))
        {
            puts(ent[i].name);
            ENTER();
        }
    }
}

static void catfile(fat_entry_t * ent)
{
    int clus = ent->fstClus;
    int size = ent->filesize[0];
    if (size == 0)
        return;
    do
    {
        int i;
        int upper = size > SECTOR_SIZE? SECTOR_SIZE: size;
        fat_entry_t * ent;
        load_sectors(sector, PHYSICAL_SECTOR(clus), 1);
        clus = fat_value(clus);
        for (i = 0; i < upper; i++)
            putc(sector[i]);
        size -= upper;
    } while (clus < THRESHOLD);
}
