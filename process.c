#define _GNU_SOURCE
#include "process.h"
#include <sched.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>


int CPU_assign(int pid, int core)
{
#ifdef DEBUG
	if (core > sizeof(cpu_set_t))
	{
		fprintf(stderr, "Core index error.");
		return -1;
	}
#endif
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core, &mask);
#ifdef DEBUG
	if (sched_setaffinity(pid, sizeof(mask), &mask) < 0)
	{
		perror("sched_setaffinity");
		exit(1);
	}
#endif
	return 0;
}

int Execute_process(PROCESS proc)
{
	int pid = fork();

	if (pid < 0)
	{
		perror("fork");
		return -1;
	}

	if (pid == 0)
	{
		//long start_sec, start_nsec, end_sec, end_nsec;
		struct timespec t1, t2;
		//gettime(&start_sec, &start_nsec);
		syscall(334, &t1);
		//syscall(GET_TIME, &start_sec, &start_nsec);
		for (int i = 0; i < proc.t_exec; i++)
		{
			TIME_UNIT();
#ifdef DEBUG
			if (i % 100 == 0)
				fprintf(stderr, "%s: %d/%d\n", proc.name, i, proc.t_exec);
#endif
		}
		char dmesg[200];
		//gettime(&end_sec, &end_nsec);
		syscall(334, &t2);
		
		sprintf(dmesg, "[Project1] %d %lu.%09lu %lu.%09lu\n", getpid(), t1.tv_sec, t1.tv_nsec, t2.tv_sec, t2.tv_nsec);

		syscall(335, dmesg);
		exit(0);
	}

	/* Assign child to another core prevent from interupted by parant */
	CPU_assign(pid, CHILD_CPU);

	return pid;
}

int Block_process(int pid)
{
	struct sched_param param;

	/* SCHED_IDLE should set priority to 0 */
	param.sched_priority = 0;

	int ret = sched_setscheduler(pid, SCHED_IDLE, &param);
#ifdef DEBUG
	if (ret < 0)
	{
		perror("sched_setscheduler");
		return -1;
	}
#endif
	return ret;
}

int Wake_process(int pid)
{
	struct sched_param param;

	/* SCHED_OTHER should set priority to 0 */
	param.sched_priority = 0;

	int ret = sched_setscheduler(pid, SCHED_OTHER, &param);
#ifdef DEBUG
	if (ret < 0)
	{
		perror("sched_setscheduler");
		return -1;
	}
#endif
	return ret;
}
