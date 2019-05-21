#ifndef SCHEDULING_UTILITY_H
#define SCHEDULING_UTILITY_H

#include <stdio.h>

processes_t* malloc_processes(int num_processes);

void free_processes(processes_t* processes);

processes_t* read_simulation_data(char* input_file);

void print_simulation_data(FILE* out, processes_t* processes);

void print_schedule(FILE* out, schedule_t schedule, processes_t* processes);

#endif // !SCHEDULING_UTILITY_H
