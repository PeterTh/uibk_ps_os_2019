#ifndef SCHEDULING_SIM_H
#define SCHEDULING_SIM_H

#include <stdbool.h>
#include <stdlib.h>

#define MAX_TIMESTEPS 1000

typedef struct {
	const char name;
	const int arrival_time;
	const int service_time;
	const int priority;
	const int remaining_time;
	// Whether this process voluntarily yielded its remaining quantum
	const bool yielded;

	int user1;
	int user2;
	int user3;
} process_t;

typedef struct {
	int num_processes;
	process_t** procs;
} processes_t;

typedef struct {
	int num_timesteps;
	char proc_names[MAX_TIMESTEPS];
	bool did_yield[MAX_TIMESTEPS];
	char* name;
} schedule_t;

typedef process_t* (*scheduling_function)(const int timestep, processes_t* procs);

#define compute_schedule(procs, sched) compute_schedule_(procs, sched, #sched)

schedule_t compute_schedule_(processes_t* processes_to_schedule, scheduling_function scheduler, char* scheduler_name);

int scheduler_rand();

#endif // !SCHEDULING_SIM_H
