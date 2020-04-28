#ifndef _SCHEDULING_H_
#define _SCHEDULING_H_

#include "process.h"
#include <string.h>

#define FIFO 292
#define RR 164
#define SJF 227
#define PSJF 307
#define P_FIFO 1
#define P_RR 2
#define P_SJF 3
#define P_PSJF 4

/* Running scheduler */
int scheduling(struct process *proc, int nproc, int policy);

#endif
