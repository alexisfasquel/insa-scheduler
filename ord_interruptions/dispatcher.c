#include "dispatcher.h"
#include "process.h"
#include "sched.h"
#include "hw.h"

//Push registre dans la pile de current_pcb

void __attribute__((naked)) ctx_switch() 
{

	//On empêche les interruptions pendant qu'on fait le changement de contexte
	DISABLE_IRQ();

	// La première instruction décrémente lr de façon à ce qu’il pointe effectivement vers l’instruction interrompue
	__asm("sub lr, lr, #4");
	
	// La deuxième instruction sauvegarde lr et le registre de statut cpsr dans la pile
	//du mode d’exécution initial du processeur (mode System).  
	__asm("srsdb sp!, #0x13");
	
	// La troisième passe en mode Superviseur. Les registres (y compris sp ...) sont donc,
	//après cela, ceux du processus interrompu
	__asm("cps #0x13");
	
	
	__asm("push {r0-r12,lr}");//On sauvegarde les registres et lr
	__asm("mov %0, sp" : "=r"(current_pcb->sp));//pour sauvegarder le registre sp dans le sp du processus courant

	
	sched();//On choisit le contexte suivant
	
	current_pcb=next_running;
	
	__asm("mov sp, %0" : : "r"(current_pcb->sp));//charge le sp du nouveau processus dans le registre sp
	
	//on ré-arme le timer
	set_next_tick_and_enable_timer_irq();
	
	//on autorise à nouveau les interruptions
	ENABLE_IRQ();
	
	if (current_pcb->etat == NEW) //Si on lance le processus pour la première fois
	{
		start_current_process(); //On le lance normalement
	}
	else
	{	
		//Sinon on récupère son contexte précédent	
		__asm("pop {r0-r12,lr}"); 
		__asm("rfefd sp!"); 	
	}
	
	__asm("bx lr"); // = return 

}

