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
static int handler_cd(fat_entry_t * entry);
static void cmd_dir();
static int handler_dir(fat_entry_t * entry);
static void cmd_cat();
static int handler_cat(fat_entry_t * entry);
static void (*find_procedure(char * cmd))();
static char sector[SECTOR_SIZE];
static void catfile(fat_entry_t * entry);
static int go_through_directory(fat_entry_t * dir, void (*handler)(fat_entry_t * sector));

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
    BEGIN_CMD();
    go_through_directory(&cd, handler_dir);
    END_CMD();
}
static int handler_dir(fat_entry_t * entry)
{
    if (IS_FREE(*entry))
        return 0;
    if (ATTR_DIRECTORY(*entry) || ATTR_ARCHIVE(*entry))
    {
        puts(entry->name);
        ENTER();
    }
    return 0;
}

static void cmd_cd()
{
    BEGIN_CMD();
    if (go_through_directory(&cd, handler_cd))
        END_CMD();
    puts("No such directory!");
    ENTER();
    END_CMD();
}
static int handler_cd(fat_entry_t * entry)
{
    if (IS_FREE(*entry))
        return 0;
    if (ATTR_DIRECTORY(*entry) || ATTR_ARCHIVE(*entry))
        if (strncmp(entry->name, buffer+3, strlen(buffer+3)) == 0)
        {
            cd = *entry;
            return 1;
        }
    return 0;
}

static void cmd_cat()
{
    BEGIN_CMD();
    if (go_through_directory(&cd, handler_cat))
        END_CMD();
    puts("No such file!");
    ENTER();
    END_CMD();
}
static int handler_cat(fat_entry_t * entry)
{
    if (IS_FREE(*entry))
        return 0;
    if (ATTR_DIRECTORY(*entry) || ATTR_ARCHIVE(*entry))
        if (strncmp(entry->name, buffer+4, strlen(buffer+4)) == 0)
        {
            catfile(entry);
            ENTER();
            return 1;
        }
    return 0;
}

static void catfile(fat_entry_t * entry)
{
    int clus = entry->fstClus;
    int size = entry->filesize[0];
    if (size == 0)
        return;
    do
    {
        int i;
        int upper = size > SECTOR_SIZE? SECTOR_SIZE: size;
        load_sectors(sector, PHYSICAL_SECTOR(clus), 1);
        clus = fat_value(clus);
        for (i = 0; i < upper; i++)
            putc(sector[i]);
        size -= upper;
    } while (clus < THRESHOLD);
}

static int go_through_directory(fat_entry_t * dir, int (*handler)(fat_entry_t * sector))
{
    int i, j;
    if (IS_ROOT(*dir))
    {
        for (i = 19; i < 33; i++)
        {
            load_sectors(sector, i, 1);
            for (j = 0; j < SECTOR_SIZE / sizeof(fat_entry_t); j++)
                if (handler(((fat_entry_t *)sector)+j))
                    return 1;
        }
    }
    else
    {
        int clus = dir->fstClus;
        do
        {
            load_sectors(sector, PHYSICAL_SECTOR(clus), 1);
            clus = fat_value(clus);
            for (j = 0; j < SECTOR_SIZE / sizeof(fat_entry_t); j++)
                if (handler(((fat_entry_t *)sector)+j))
                    return 1;
        } while (clus < THRESHOLD);
    }
    return 0;
}
