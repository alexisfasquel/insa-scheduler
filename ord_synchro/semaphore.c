#include "semaphore.h"
#include "process.h"
#include "dispatcher.h"
#include "hw.h"

void sem_init(struct sem_s* sem, unsigned int val)
//initialise le sémaphore
{
	sem->compteur = val;
	sem->blocked_pcb=0;
	
}

void sem_up(struct sem_s* sem)
//Ajoute un ticket au sémaphore et débloque un processus bloqué (si il y en a)
{
	DISABLE_IRQ(); //on arrête les interruptions 
	sem->compteur += 1; 

	if (sem->compteur <= 0 ) //Si on débloque qqun, on le remet dans la liste des pcb ready
	{

		struct pcb_s* temp= sem->blocked_pcb->next;
	
		sem->blocked_pcb->next = current_pcb->next;
	
		current_pcb->next = sem->blocked_pcb;

		current_pcb->etat = READY;
		
		sem->blocked_pcb = temp;


	}
	ENABLE_IRQ(); // on réactive les interruptions
	
}

void sem_down(struct sem_s* sem)
//Retire un ticket au sémaphore et bloque le processus appelant si il n'y avait pas assez de ticket
{
	DISABLE_IRQ();//on arrête les interruptions pour pas qu'on soit interrompu en changeant les processus de file
	sem->compteur -= 1;

	if (sem->compteur < 0 ) //Si on bloque qqun, on le met dans la liste des pcb bloqués du sémaphore
	{

		struct pcb_s* pcb_a_bloquer=current_pcb;
		pcb_a_bloquer->etat = WAITING;

		struct pcb_s* pcb_precedent=pcb_a_bloquer;

		while(pcb_precedent->next != pcb_a_bloquer) // cherche le pcb précédent le current
		{
			pcb_precedent=pcb_precedent->next;
		}

		//On rajoute le pcb en tête de la liste des pcb bloqués
		pcb_precedent->next = pcb_a_bloquer->next;
	
		pcb_a_bloquer->next = sem->blocked_pcb;
		
		sem->blocked_pcb = pcb_a_bloquer;


		yield(pcb_precedent->next);

	}
	else
	{
		ENABLE_IRQ();// on réactive les interruptions
	}


}

void mtx_init(struct mtx_s* mutex) 
{
	mutex->compteur = 1;
	mutex->blocked_pcb = 0;
}

//Verrouille le mutex ou bloque le process appellant si déDISABLE_IRQ(); //on arrête les interruptions ja verrouillé
void mtx_lock(struct mtx_s* mutex) 
{
	DISABLE_IRQ(); //on arrête les interruptions 
	
	if(current_pcb != mutex->owner_pcb)
	{
		if(mutex->compteur!=1) {
		
			current_pcb->etat = WAITING;

			struct pcb_s* pcb_precedent=current_pcb;

			while(pcb_precedent->next != current_pcb) // cherche le pcb précédent le current
			{
				pcb_precedent=pcb_precedent->next;
			}

		
			pcb_precedent->next = current_pcb->next; //Sort current_pcb de la liste
	
			current_pcb->next = mutex->blocked_pcb; // le suivant de current pcb devient la tête de liste des pcb bloqués
		
			mutex->blocked_pcb = current_pcb; // la nouvelle tete de liste des pcb bloqués devient current pcb

			yield(pcb_precedent->next); //et vu que le current a été bloqué, on passe au suivant
		
		}

		mutex->compteur -= 1;
		mutex->owner_pcb = current_pcb;
	}

	ENABLE_IRQ();// on réactive les interruptions

}

void mtx_unlock(struct mtx_s* mutex)
{
	DISABLE_IRQ(); //on arrête les interruptions 

	if(mutex->owner_pcb == current_pcb) {
		
		mutex->compteur += 1;
		mutex->owner_pcb = 0;

		// on remet le pcb bloqué 
		struct pcb_s* pcb_blocked_next;
		
		pcb_blocked_next = mutex->blocked_pcb->next;
	
		mutex->blocked_pcb->next = current_pcb->next;
	
		current_pcb->next = mutex->blocked_pcb;

		current_pcb->next->etat = READY;
		
		mutex->blocked_pcb = pcb_blocked_next;
	}

	ENABLE_IRQ();// on réactive les interruptions
}









