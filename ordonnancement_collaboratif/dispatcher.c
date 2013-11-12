#include "dispatcher.h"
#include "process.h"
#include "sched.h"


//Push registre dans la pile de current_pcb

void __attribute__((naked)) ctx_switch() 
{
	__asm("push {r0-r12,lr}");
	__asm("mov %0, sp" : "=r"(current_pcb->sp));
	sched();
	current_pcb=next_running;
	__asm("mov sp, %0" : : "r"(current_pcb->sp));

	if (current_pcb->etat == NEW)
	{
		start_current_process();
	}
	else
	{
		__asm("pop {r0-r12,lr}"); //pt bug on sait pasq pq
	}


}

