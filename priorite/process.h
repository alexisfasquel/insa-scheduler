#ifndef __PROCESS_H
#define __PROCESS_H

#include "malloc.h"
#include <stdint.h>

typedef enum {NEW, READY, RUNNING, WAITING, TERMINATED} state; //Etat d'un processus

typedef void (*func_t) ( void);



struct pcb_s //Structure d'un processus
{

	uint32_t* sp; //Pointeur de pile du processus

	uint32_t* stack_begin; // Pointeur sur le début de la pile du processus (nécessaire pour la suppression)
	
	func_t f; //Fonction du processus

	void * args; //Arguments de la fonction du processus
	
	struct pcb_s* next; //Processus suivant

	struct pcb_s* prev; //Processus precédent

	state etat; //Etat du processus
	
	int priorite;

	int temps_reveil;
	
	
};



extern struct pcb_s* current_pcb; //processus courant
extern struct pcb_s* head;//tête de la liste des processus ready
extern struct pcb_s* parcours;//fin de la liste des processus

void init_pcb(struct pcb_s* pcb, unsigned int stack_size,int prioritepcb); //Alloue la place nécessaire pour la pile du processus


#endif

