#ifndef _CONFIG_H
#define _CONFIG_H

#define STKSZ 512
#define MAXTSK 16
#define ENTER_CRITICAL() asm "cli"
#define EXIT_CRITICAL() asm "sti"

#endif
