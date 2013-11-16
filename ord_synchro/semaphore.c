#include "semaphore.h"
#include "process.h"
#include "dispatcher.h"

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

void mtx_init(struct mtx_s* mutex) {

	mutex->compteur = 1;
	mutex->blocked_pcb = 0;
}

//Verrouille le mutex ou bloque le process appellant si déja verrouillé
void mtx_lock(struct mtx_s* mutex) {

	if(mutex->compteur == 1) {
		
		mutex->compteur -= 1;
		mutex->owner_pcb = current_pcb;
	}
	else if(current_pcb != mutex->owner_pcb) {
		
		current_pcb->etat = WAITING;

		struct pcb_s* temp=current_pcb;

		while(temp->next != current_pcb) // cherche le pcb précédent le current
		{
			temp=temp->next;
		}

		
		temp->next = current_pcb->next; //Sort current_pcb de la liste
	
		current_pcb->next = mutex->blocked_pcb; // le suivant de current pcb devient la tête de liste des pcb bloqués
		
		mutex->blocked_pcb = current_pcb; // la nouvelle tete de liste des pcb bloqués devient current pcb

		current_pcb=temp; // Current pcb devient 

		ctx_switch(); //et vu que le current a été bloqué, on passe au suivant
	}
}
void mtx_unlock(struct mtx_s* mutex) {

	if(mutex->owner_pcb == current_pcb) {
		
		mutex->compteur += 1;
		mutex->owner_pcb = 0;

		// on remet le pcb bloqué 
		struct pcb_s* temp;
		
		temp = mutex->blocked_pcb->next;
	
		mutex->blocked_pcb->next = current_pcb->next;
	
		current_pcb->next = mutex->blocked_pcb;

		current_pcb->etat = READY;
		
		mutex->blocked_pcb = temp;
	}
}


void philosophe(int i) //A REVOIR : Principe de l'algo du philosophe, en c++ ici
//d'après cours de Maranzana de l'année dernière
{
	for ( ; ; )
	{
		penser();
		mtx_lock(mtx); //protection du code
		prendreFourchette(i);
		prendreFourchette( (i-1)? i-1 : MAX);
		manger();
		poserFourchette(i);
		poserFourchette((i-1) ? i-1 : MAX);
		mtx_unlock(mtx);
	}
}




