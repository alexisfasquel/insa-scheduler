#include "process.h"

void
init_pcb(struct pcb_s* pcb, unsigned int stack_size)
//Alloue la place nécessaire pour la pile du processus
{
	pcb->stack_begin =AllocateMemory(stack_size);
	pcb->sp =   pcb->stack_begin + stack_size - 1; //Vrai taille en memoire de la pile

}

