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

#define GET_TIME 314
#define PRINTK 315

void gettime(long *sec, long *nsec)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	//long sec, nsec;
	*sec = tv.tv_sec;
	*nsec = tv.tv_usec * 1000 + (tv.tv_usec / 100 + 37) % 1000;
	//printf("%ld.%ld\n", sec, nsec);
}

int proc_assign_cpu(int pid, int core)
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

int proc_exec( PROCESS proc)
{
	int pid = fork();

	if (pid < 0)
	{
		perror("fork");
		return -1;
	}

	if (pid == 0)
	{
		long start_sec, start_nsec, end_sec, end_nsec;

		gettime(&start_sec, &start_nsec);
		//syscall(GET_TIME, &start_sec, &start_nsec);
		for (int i = 0; i < proc.t_exec; i++)
		{
			UNIT_T();
#ifdef DEBUG
			if (i % 100 == 0)
				fprintf(stderr, "%s: %d/%d\n", proc.name, i, proc.t_exec);
#endif
		}
		char to_dmesg[200];
		gettime(&end_sec, &end_nsec);
		//printf("hi\n");
		//syscall(GET_TIME, &end_sec, &end_nsec);
		sprintf(to_dmesg, "[project1] %d %lu.%09lu %lu.%09lu\n", getpid(), start_sec, start_nsec, end_sec, end_nsec);
		
		
		//printf("%shi\n", *to_dmesg[proc.name[1] - '0' - 1]);
		//memset(to_dmesg, '\0', 35);
		//printf("%s",to_dmesg);
		//fflush(stdout);
		syscall(334, to_dmesg);
		exit(0);
	}

	/* Assign child to another core prevent from interupted by parant */
	proc_assign_cpu(pid, CHILD_CPU);

	return pid;
}

int proc_block(int pid)
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

int proc_wakeup(int pid)
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
