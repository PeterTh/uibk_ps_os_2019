#include "scheduling_sim.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_LEN_SCHEDULING_FUN 10

static int match_lines(FILE* fp, processes_t* procs) {
	int line = 0;
	process_t tmp = {0};

	while(fscanf(fp, "%c;%d;%d;%d\n", (char*)&tmp.name, (int*)&tmp.arrival_time, (int*)&tmp.service_time,
	          (int*)&tmp.priority)
	      == 4) {
		if(procs) {
			process_t* p = malloc(sizeof(*p));
			memcpy(p, &tmp, sizeof(tmp));
			procs->procs[line] = p;
		}
		++line;
	}
	rewind(fp);
	return line;
}

processes_t* malloc_processes(int num_processes) {
	processes_t* processes = malloc(sizeof(*processes));
	if(!processes) { return NULL; }

	process_t** process = malloc(sizeof(*process) * num_processes);
	if(!process) {
		free(processes);
		return NULL;
	}

	processes->num_processes = num_processes;
	processes->procs = process;
	return processes;
}

void free_processes(processes_t* processes) {
	for(int i = 0; i < processes->num_processes; ++i) {
		free(processes->procs[i]);
	}
	free(processes->procs);
	free(processes);
}

processes_t* read_simulation_data(char* input_file) {
	FILE* fp = fopen(input_file, "r");
	if(!fp) { return NULL; }
	int num_processes = match_lines(fp, NULL);
	processes_t* processes = malloc_processes(num_processes);
	match_lines(fp, processes);
	fclose(fp);
	return processes;
}

void print_simulation_data(FILE* out, processes_t* processes) {
	fprintf(out, "simulation data:\n");
	fprintf(out, "name\tarrival_time\tservice_time\tpriority\n");
	for(int i = 0; i < processes->num_processes; ++i) {
		process_t* proc = processes->procs[i];
		printf("%c    \t%12u\t%12u\t%8u\n", proc->name, proc->arrival_time, proc->service_time, proc->priority);
	}
	fprintf(out, "\n");
}

void print_schedule(FILE* out, schedule_t sched, processes_t* processes) {
	int offset_len = strlen(sched.name) / 2;
	fprintf(out, "===================================");
	fprintf(out, "%*s%*s", MAX_LEN_SCHEDULING_FUN + offset_len, sched.name, MAX_LEN_SCHEDULING_FUN - offset_len, "");
	fprintf(out, "===================================\n\n");

	// header
	for(int i = 0; i < sched.num_timesteps; i++) {
		fprintf(out, " %2d ", i);
	}
	fprintf(out, "\n");

	int start_time[256];
	int end_time[256] = {0};

	for(int i = 0; i < 256; ++i) {
		start_time[i] = sched.num_timesteps;
	}

	for(int i = 0; i < sched.num_timesteps; i++) {
		int cur = (int)sched.proc_names[i];
		if(i < start_time[cur]) { start_time[cur] = i; }
		if(i > end_time[cur]) { end_time[cur] = i; }
	}

	int is_terminal = isatty(fileno(out));
	const char* ansi_green = is_terminal ? "\033[92m" : "";
	const char* ansi_red = is_terminal ? "\033[91m" : "";
	const char* ansi_reset = is_terminal ? "\033[0m" : "";
	// schedule
	for(int i = 0; i < sched.num_timesteps; i++) {
		char proc = sched.proc_names[i];
		const char* color = start_time[(int)proc] == i ? ansi_green : (end_time[(int)proc] == i ? ansi_red : "");
		fprintf(out, "  %s%c%s ", color, proc, ansi_reset);
	}
	fprintf(out, "\n");
	for(int i = 0; i < sched.num_timesteps; i++) {
		char did_yield = sched.did_yield[i];
		fprintf(out, "  %c ", did_yield ? 'Y' : ' ');
	}
	fprintf(out, "\n\n");

	fprintf(out, "name\tresponse_time\tturnaround_time\n");
	for(int i = 0; i < processes->num_processes; ++i) {
		int arrival_time = processes->procs[i]->arrival_time;
		int name = processes->procs[i]->name;
		fprintf(
		    out, "%c   \t%13d\t%16d\n", name, start_time[name] - arrival_time, end_time[name] - start_time[name] + 1);
	}

	fprintf(out, "\n\n\n");
}
