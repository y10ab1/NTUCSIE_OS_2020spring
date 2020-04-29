#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <sys/types.h>
#include <string.h>

#define CHILD_CPU 1
#define PARENT_CPU 0

/* Running one unit time */
#define TIME_UNIT()                        \
	{                                   \
		volatile unsigned long i;       \
		for (i = 0; i < 1000000UL; i++) \
			;                           \
	}

typedef struct process
{
	char name[32];
	int t_ready;
	int t_exec;
	pid_t pid;
} PROCESS;


/*policy implement*/
void policy_SJF_PSJF(int policy, int nproc, PROCESS *proc, int *ret);
void policy_RR(int policy, int nproc, PROCESS *proc, int *ret);
void policy_FIFO(int policy, int nproc, PROCESS *proc, int *ret);

/* Assign process to specific core */
int CPU_assign(int pid, int core);

/* Exec process and return pid */
int Execute_process( PROCESS proc);

/* Set very low priority tp process */
int Block_process(int pid);

/* Set high priority to process */
int Wake_process(int pid);

#endif
