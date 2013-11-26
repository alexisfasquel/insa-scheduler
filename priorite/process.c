#include "process.h"



/*ajouter le processus ds la liste fifo qui coresspond à sa priorite*/
void
init_pcb(struct pcb_s* pcb, unsigned int stack_size,int prioritepcb)
{
	pcb->stack_begin =(uint32_t*)  malloc_alloc(stack_size);
	pcb->sp =   pcb->stack_begin +  (stack_size / sizeof(uint32_t)) - 1; //Vrai taille de la memoire de la pile
 	pcb->priorite = prioritepcb;
	pcb->temps_reveil=0;
}



