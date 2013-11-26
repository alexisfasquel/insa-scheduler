#ifndef SCHED_H
#define SCHED_H

#include "process.h"

void create_process(int stack_size, func_t f, void* args,int prioritepcb); //Cree un processus et le rajoute dans la liste des processus ready
void start_current_process();
void start_sched(); //On utilise un processus inutile au début pour qu'on ait un current_pcb avant de commencer
void sched();//Supprimer les processus terminés, et choisit le processus suivant
void sleep(int number);
void checkwaiting();

extern struct pcb_s* next_running; //prochain processus à executer
extern struct pcb_s* useless_pcb; // processus inutile au début pour qu'on ait un current_pcb avant de commencer 

#endif
