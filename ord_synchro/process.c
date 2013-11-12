#include "process.h"
#include "dispatcher.h"

void
init_pcb(struct pcb_s* pcb, unsigned int stack_size)
{
   pcb->stack_begin =AllocateMemory(stack_size);
   pcb->sp =   pcb->stack_begin + stack_size - 1; //Vrai taille de la memoire de la pile
  
}


void sem_init(struct sem_s* sem, unsigned int val)
//initialise le sémaphore
{

	sem->compteur = val;
	sem->blocked_pcb =0;
}

void sem_up(struct sem_s* sem)
//Ajoute un ticket au sémaphore et débloque un processus bloqué (si il y en a)
{
	sem->compteur += 1; 

	if (sem->compteur <= 0 ) //Si on débloque qqun, on le remet dans la liste des pcb ready
	{

		struct pcb_s* temp;
		
		temp = sem->blocked_pcb->next;
	
		sem->blocked_pcb->next = current_pcb->next;
	
		current_pcb->next = sem->blocked_pcb;

		current_pcb->etat = READY;
		
		sem->blocked_pcb = temp;
	

	}
}

void sem_down(struct sem_s* sem)
//Retire un ticket au sémaphore et bloque le processus appelant si il n'y avait pas assez de ticket
{
	sem->compteur -= 1;

	if (sem->compteur < 0 ) //Si on bloque qqun, on le met dans la liste des pcb bloqués du sémaphore
	{

		current_pcb->etat = WAITING;

		struct pcb_s* temp=current_pcb;

		while(temp->next != current_pcb) // cherche le pcb précédent le current
		{
			temp=temp->next;
		}

		
		temp->next = current_pcb->next;
	
		current_pcb->next = sem->blocked_pcb;
		
		sem->blocked_pcb = current_pcb;

		current_pcb=temp;

		ctx_switch(); //et vu que le current a été bloqué, on passe au suivant

	}

}
