#include "dispatcher.h"
#include "process.h"
#include "sched.h"


//Push registre dans la pile de current_pcb

void __attribute__((naked)) ctx_switch() 
{
	__asm("push {r0-r12,lr}"); //On sauvegarde les registres et lr
	__asm("mov %0, sp" : "=r"(current_pcb->sp)); //pour sauvegarder le registre sp dans le sp du processus courant
	sched(); //On choisit le contexte suivant
	current_pcb=next_running; 
	__asm("mov sp, %0" : : "r"(current_pcb->sp)); //charge le sp du processus dans le registre sp

	if (current_pcb->etat == NEW) //Si on lance le processus pour la première fois
	{
		start_current_process(); //On le lance normalement
	}
	else
	{
		//Sinon on récupère son contexte précédent
		__asm("pop {r0-r12,lr}"); 
	}


}

