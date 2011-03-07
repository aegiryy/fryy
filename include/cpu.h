#ifndef _CPU_H
#define _CPU_H

#define IDXAX 0
#define IDXBX 1
#define IDXCX 2
#define IDXDX 3
#define IDXSP 4
#define IDXBP 5
#define IDXSI 6
#define IDXDI 7
#define IDXCS 8
#define IDXDS 9
#define IDXSS 10
#define IDXES 11
#define IDXIP 12
#define IDXFLG 13
#define REGCNT 14

#define ENTER_CRITICAL() asm "cli"
#define EXIT_CRITICAL() asm "sti"

#endif
