#include "scheduling_sim.h"
#include "scheduling_utility.h"

#include <assert.h>

void init_processes(processes_t* processes_to_schedule) {
	for(int i = 0; i < processes_to_schedule->num_processes; ++i) {
		process_t* cur = processes_to_schedule->procs[i];
		*(int*)(&cur->remaining_time) = cur->service_time;
		cur->user1 = 0;
		cur->user2 = 0;
		cur->user3 = 0;
	}
}

int _sched_rng_state;

int scheduler_rand() {
	srand(_sched_rng_state);
	return rand();
}

schedule_t compute_schedule_(processes_t* processes_to_schedule, scheduling_function scheduler, char* scheduler_name) {
	init_processes(processes_to_schedule);

	schedule_t sched;
	int timestep = 0;

	processes_t* procs_to_pass = malloc_processes(processes_to_schedule->num_processes);

	process_t* current = NULL;
	for(; timestep < MAX_TIMESTEPS - 1; ++timestep) {
		_sched_rng_state = timestep;

		procs_to_pass->num_processes = 0;
		for(int i = 0; i < processes_to_schedule->num_processes; ++i) {
			process_t* p = processes_to_schedule->procs[i];
			if(timestep >= p->arrival_time && p->remaining_time > 0) {
				procs_to_pass->procs[procs_to_pass->num_processes++] = p;
				if(p != current) {
					bool* yielded = (bool*)(&p->yielded);
					*yielded = false;
				}
			}
		}

		// if nothing left to schedule we're done
		if(procs_to_pass->num_processes == 0) { break; }

		current = scheduler(timestep, procs_to_pass);
		assert(current != NULL);

		// randomly decide whether this process yields
		bool* yielded = (bool*)(&current->yielded);
		*yielded = scheduler_rand() % 3 == 0;

		int* rem_time = (int*)(&current->remaining_time);
		(*rem_time)--;

		// record the schedule
		sched.proc_names[timestep] = current->name;
		sched.did_yield[timestep] = current->yielded;
	}

	sched.num_timesteps = timestep;
	sched.proc_names[timestep] = '\0';
	sched.name = scheduler_name;

	free_processes(procs_to_pass);
	return sched;
}
