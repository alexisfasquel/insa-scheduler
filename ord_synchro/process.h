#ifndef __PROCESS_H
#define __PROCESS_H

#include "allocateMemory.h"

typedef enum {NEW, READY, RUNNING, WAITING, TERMINATED} state; //Etat d'un processus

typedef void (*func_t) ( void);

struct pcb_s //Structure d'un processus
{

	uint32_t* sp; //Pointeur de pile du processus

	uint32_t* stack_begin; // Pointeur sur le début de la pile du processus (nécessaire pour la suppression)
	
	func_t f; //Fonction du processus
	void * args; //Arguments de la fonction du processus
	
	struct pcb_s* next; //Processus suivant

	state etat; //Etat du processus
	
};

//Struture semaphore
struct sem_s 
{
	
	int compteur;
	struct pcb_s* blocked_pcb;
};

extern struct pcb_s* current_pcb; //processus courant
extern struct pcb_s* head;//tête de la liste des processus ready

void init_pcb(struct pcb_s* pcb, unsigned int stack_size); //Alloue la place nécessaire pour la pile du processus

void sem_init(struct sem_s* sem, unsigned int val); //Initialise le sémaphore

void sem_up(struct sem_s* sem); //Ajoute un ticket au sémaphore et débloque un processus bloqué (si il y en a)
void sem_down(struct sem_s* sem); //Retire un ticket au sémaphore et bloque le processus appelant si il n'y avait pas assez de ticket

#endif

