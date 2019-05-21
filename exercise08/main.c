#include <limits.h>
#include <stdio.h>

#include "scheduling_sim.h"
#include "scheduling_utility.h"

// ---------------------------------------------------------------------------
// Example schedulers
// ---------------------------------------------------------------------------

process_t* fcfs(const int __attribute__((unused)) timestep, processes_t* processes) {
	process_t* selected = NULL;
	int min_arrival_time = INT_MAX;

	for(int i = 0; i < processes->num_processes; ++i) {
		process_t* proc = processes->procs[i];
		if(proc->yielded) {
			proc->user1 = 0;
			continue;
		}

		if(proc->user1 == 1) { return proc; }

		if(proc->arrival_time < min_arrival_time) {
			min_arrival_time = proc->arrival_time;
			selected = proc;
		}
	}

	if(selected == NULL) { selected = processes->procs[0]; }
	selected->user1 = 1;
	return selected;
}

process_t* round_robin(const int __attribute__((unused)) timestep, processes_t* processes_struct) {
	process_t** processes = processes_struct->procs;
	int num_processes = processes_struct->num_processes;

	process_t* selected = processes[0];
	int next_idx = 1 % num_processes;
	for(int i = 0; i < num_processes; ++i) {
		if(processes[i]->user1 == 1) {
			selected = processes[i];
			next_idx = (i + 1) % num_processes;
			break;
		}
	}

	selected->user1 = 0;
	process_t* next = processes[next_idx];
	if(next != selected) { next->user1 = 1; }
	return selected;
}

// ---------------------------------------------------------------------------
// Implement your schedulers here
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------

int main(int argc, char** argv) {
	if(argc < 2) {
		fprintf(stderr, "Error: Usage: %s <filename>\nExample: %s input.csv\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	processes_t* procs = read_simulation_data(argv[1]);
	print_simulation_data(stdout, procs);

	// -----------------------------------------------------------------------
	// Insert calls to your schedulers here
	// -----------------------------------------------------------------------

	print_schedule(stdout, compute_schedule(procs, fcfs), procs);
	print_schedule(stdout, compute_schedule(procs, round_robin), procs);

	// -----------------------------------------------------------------------

	free_processes(procs);

	return EXIT_SUCCESS;
}
