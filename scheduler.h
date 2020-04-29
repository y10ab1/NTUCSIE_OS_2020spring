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

/* Last context switch time for RR scheduling */
static int t_last;

/* Current unit time */
static int now_time;

/* Index of running process. -1 if no process running */
static int running;

/* Number of finish Process */
static int finish_count;

/* Running scheduler */
int scheduling(struct process *proc, int nproc, int policy);

/*policy implement*/
void policy_SJF_PSJF(int policy, int nproc, PROCESS *proc, int *ret);
void policy_RR(int policy, int nproc, PROCESS *proc, int *ret);
void policy_FIFO(int policy, int nproc, PROCESS *proc, int *ret);

#endif
