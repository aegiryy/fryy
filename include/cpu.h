#ifndef _CPU_H
#define _CPU_H

#define IDXFLG 0
#define IDXCS 1
#define IDXIP 2
#define IDXAX 3
#define IDXBX 4
#define IDXCX 5
#define IDXDX 6
#define IDXBP 7
#define IDXSI 8
#define IDXDI 9
#define IDXDS 10
#define IDXES 11
#define REGCNT 14

#define ENTER_CRITICAL() asm "cli"
#define EXIT_CRITICAL() asm "sti"

#endif
