#ifndef SCHED_H
#define SCHED_H

#include "process.h"

void create_process(int stack_size, func_t f, void* args);
void start_current_process();
void start_sched();
void sched();

struct pcb_s* next_running; //prochain processus à executer
struct pcb_s* useless_pcb; 

#endif
