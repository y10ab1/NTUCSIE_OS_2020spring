#define _GNU_SOURCE
#include "process.h"
#include "scheduler.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>



/* Sort processes by ready time */
int cmp(const void *a, const void *b)
{
	return ((PROCESS *)a)->t_ready - ((PROCESS *)b)->t_ready;
}

/* Return index of next process  */
int next_process(PROCESS *proc, int nproc, int policy)
{
	/* Non-preemptive */
	if (running != -1 && (policy == SJF || policy == FIFO))
		return running;

	int ret = -1;

	switch (policy)
	{
	case P_PSJF:
		policy_SJF_PSJF(policy, nproc, proc, &ret);
		break;
	case P_SJF:
		policy_SJF_PSJF(policy, nproc, proc, &ret);
		break;
	case P_FIFO:
		policy_FIFO(policy, nproc, proc, &ret);
		break;
	case P_RR:
		policy_RR(policy, nproc, proc, &ret);
		break;

	default:
		break;
	}

	return ret;
}

int scheduling(PROCESS *proc, int nproc, int policy)
{
	qsort(proc, nproc, sizeof(PROCESS), cmp);

	/* Initial pid = -1 imply not ready */
	for (int i = 0; i < nproc; i++)
		proc[i].pid = -1;

	pid_t this_pid = getpid();
	/* Set single core prevent from preemption */
	CPU_assign(this_pid, 0);

	/* Set high priority to scheduler */
	Wake_process(this_pid);

	/* Initial scheduler */
	finish_count = 0;
	now_time = 0;
	running = -1;

	while (1)
	{
		//fprintf(stderr, "Current time: %d\n", now_time);

		/* Check if running process finish */
		if (running != -1 && proc[running].t_exec == 0)
		{

#ifdef DEBUG
			fprintf(stderr, "%s finish at time %d.\n", proc[running].name, now_time);
#endif

			waitpid(proc[running].pid, NULL, 0);
			printf("%s %d\n", proc[running].name, proc[running].pid);
			running = -1;
			finish_count++;

			/* All process finish */
			if (finish_count == nproc)
				break;
		}

		/* Check if process ready and execute */
		for (int i = 0; i < nproc; i++)
		{
			if (proc[i].t_ready == now_time)
			{

				proc[i].pid = Execute_process(proc[i]);

				Block_process(proc[i].pid);
#ifdef DEBUG
				fprintf(stderr, "%s ready at time %d.\n", proc[i].name, now_time);
#endif
			}
		}

		/* Select next running  process */
		int next = next_process(proc, nproc, policy);
		if (next != -1)
		{
			/* Context switch */
			if (running != next)
			{
				Wake_process(proc[next].pid);
				Block_process(proc[running].pid);
				running = next;
				t_last = now_time;
			}
		}

		/* Run an unit of time */
		TIME_UNIT();
		if (running != -1)
			proc[running].t_exec--;
		now_time++;
	}

	return 0;
}

void policy_SJF_PSJF(int policy, int nproc, PROCESS *proc, int *ret)
{
	if (policy == P_PSJF || policy == P_SJF)
	{
		for (int i = 0; i < nproc; i++)
		{
			if (proc[i].pid == -1 || proc[i].t_exec == 0)
				continue;
			if (*ret == -1 || proc[i].t_exec < proc[*ret].t_exec)
				*ret = i;
		}
	}
}
void policy_RR(int policy, int nproc, PROCESS *proc, int *ret)
{
	if (running == -1)
	{
		for (int i = 0; i < nproc; i++)
		{
			if (proc[i].pid != -1 && proc[i].t_exec > 0)
			{
				*ret = i;
				break;
			}
		}
	}
	else if ((now_time - t_last) % 500 == 0)
	{
		*ret = (running + 1) % nproc;
		while (proc[*ret].pid == -1 || proc[*ret].t_exec == 0)
			*ret = (*ret + 1) % nproc;
	}
	else
		*ret = running;
}
void policy_FIFO(int policy, int nproc, PROCESS *proc, int *ret)
{
	for (int i = 0; i < nproc; i++)
	{
		if (proc[i].pid == -1 || proc[i].t_exec == 0)
			continue;
		if (*ret == -1 || proc[i].t_ready < proc[*ret].t_ready)
			*ret = i;
	}
}