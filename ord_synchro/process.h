#ifndef __PROCESS_H
#define __PROCESS_H

#include "allocateMemory.h"

typedef enum {NEW, READY, RUNNING, WAITING, TERMINATED} state;

typedef void (*func_t) ( void);

struct pcb_s {

	uint32_t* sp;

	uint32_t* stack_begin;
	
	void * args;
	func_t f;

	struct pcb_s* next;

	state etat;
	
};

//Struture semaphore
struct sem_s {
	
	int compteur;
	struct pcb_s* blocked_pcb;
};

extern struct pcb_s* current_pcb; //pcb courant
extern struct pcb_s* head;//tête de la liste

void init_pcb(struct pcb_s* pcb, unsigned int stack_size);

void sem_init(struct sem_s* sem, unsigned int val);

void sem_up(struct sem_s* sem);
void sem_down(struct sem_s* sem);

#endif

