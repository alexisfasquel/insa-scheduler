#include "process.h"

void
init_pcb(struct pcb_s* pcb, unsigned int stack_size)
{
	pcb->stack_begin = (uint32_t*) malloc_alloc(stack_size);
	pcb->sp =  pcb->stack_begin + (stack_size / sizeof(uint32_t)) - 1; //Vrai taille de la memoire de la pile
 
}

