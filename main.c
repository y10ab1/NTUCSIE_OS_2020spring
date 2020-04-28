#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include "process.h"
#include "scheduler.h"

char to_dmesg[10][200];

int main(int argc, char *argv[])
{
	PROCESS *proc;
	int policy;
	int nproc;
	char sch_policy[5];
	int index = 0;
	int i = 0;
	scanf("%s", sch_policy);
	scanf("%d", &nproc);

	while (sch_policy[i] != '\0')
	{
		index += sch_policy[i++];
	}

	proc = (PROCESS *)malloc(nproc * sizeof(PROCESS));

	for (int i = 0; i < nproc; i++)
	{
		scanf("%s%d%d", proc[i].name,
			  &proc[i].t_ready, &proc[i].t_exec);
	}

	switch (index)
	{
	case FIFO:
		policy = P_FIFO;
		scheduling(proc, nproc, policy);

		break;
	case RR:
		policy = P_RR;
		scheduling(proc, nproc, policy);

		break;
	case SJF:
		policy = P_SJF;
		scheduling(proc, nproc, policy);

		break;
	case PSJF:
		policy = P_PSJF;
		scheduling(proc, nproc, policy);

		break;
	default:
		break;
	}
	/*
	for (int i = 0; i < 11; ++i)
	{
		printf("%s\n", to_dmesg[i]);
	}



	*/
	exit(0);
	/*
	if (strcmp(sch_policy, "FIFO") == 0)
	{
		policy = FIFO;
		scheduling(proc, nproc, policy);
		exit(0);
	}
	else if (strcmp(sch_policy, "RR") == 0)
	{
		policy = RR;
		scheduling(proc, nproc, policy);
		exit(0);
	}
	else if (strcmp(sch_policy, "SJF") == 0)
	{
		policy = SJF;
		scheduling(proc, nproc, policy);
		exit(0);
	}
	else if (strcmp(sch_policy, "PSJF") == 0)
	{
		policy = PSJF;
		scheduling(proc, nproc, policy);
		exit(0);
	}
	else
	{
		fprintf(stderr, "Invalid policy: %s", sch_policy);
		exit(0);
	}
	*/
	/*
	scheduling(proc, nproc, policy);

	exit(0);*/
}
