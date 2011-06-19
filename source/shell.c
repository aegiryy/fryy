#include "shell.h"
#include "io.h"
#include <strings.h>

static int _task_func = 0;
static char buffer[BUFSZ];
static void cmd_echo();
static void cmd_exit();
static void (*find_procedure(char * cmd))();

void shell()
{
    char c;
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
    else
        return 0;
}

static void cmd_echo()
{
    int i;
    int task_func = _task_func;
    for(i = 5; buffer[i] != '\0'; i++)
        putc(buffer[i]);
    ENTER();
    if (task_func == 0)
        task_deinit(task_get());
}

static void cmd_exit()
{
    task_deinit(task_get());
}
