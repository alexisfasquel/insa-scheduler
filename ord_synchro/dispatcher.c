#include "dispatcher.h"
#include "process.h"
#include "sched.h"
#include "hw.h"

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
	

	if (current_pcb->etat == NEW) //Si on lance le processus pour la première fois
	{
		ENABLE_IRQ();
		start_current_process(); //On le lance normalement
		
	}
	else
	{	
		//Sinon on récupère son contexte précédent	
		__asm("pop {r0-r12,lr}");
	}

	ENABLE_IRQ();// on réactive les interruptions

	__asm("rfefd sp!"); 	/* Jump to elected task, popping cpsr and pc from SVC stack  */
	/* No need to explicitly enable IRQs as 'rfe' restore cpsr */	

	
}

void __attribute__((naked)) yield(struct pcb_s* pcb_next) 
{
	//On empêche les interruptions pendant qu'on fait le changement de contexte
	DISABLE_IRQ();

	__asm("srsdb sp!, #0x13"); //On doit sauvegarder les mêmes trucs que dans ctx_switch (lr et cpsr) 
				//pcq sinon on n'aurait pas les mêmes infos dans les ctx dans ctx_switch et dans yield
	__asm("push {r0-r12,lr}"); //On sauvegarde les registres et lr
	__asm("mov %0, sp" : "=r"(current_pcb->sp)); //pour sauvegarder le registre sp dans le sp du processus courant
	
	if (pcb_next==	0)
	{
		sched(); //On choisit le contexte suivant
		current_pcb=next_running; 
	}
	else
	{
		current_pcb=pcb_next;
	}
	
	
	__asm("mov sp, %0" : : "r"(current_pcb->sp)); //charge le sp du nouveau processus dans le registre sp

	if (current_pcb->etat == NEW) //Si on lance le processus pour la première fois
	{
		ENABLE_IRQ()
		start_current_process(); //On le lance normalement
	}
	else
	{
		//Sinon on récupère son contexte précédent
		__asm("pop {r0-r12,lr}"); 
	}

	ENABLE_IRQ();// on réactive les interruptions

	__asm("rfefd sp!"); 	// Vu qu'on a rajouté lr et cpsr, il faut les enlever /* Jump to elected task, popping cpsr and pc from SVC stack  */

	//__asm("bx lr");
}

