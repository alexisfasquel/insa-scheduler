#include "sched.h"
#include "dispatcher.h"
#include "process.h"
#include "hw.h"

#define STACK_SIZE 12800
#define NULL 0

struct pcb_s* head=NULL;
struct pcb_s* current_pcb;


void create_process(int stack_size,func_t f, void* args)
{
	struct pcb_s* newpcb =  (struct pcb_s*)AllocateMemory(sizeof(struct pcb_s));
	init_pcb(newpcb, STACK_SIZE);
	newpcb->args=args;
	newpcb->etat = NEW;
	newpcb->f = (void*)f;
	if(head == NULL)
	{
		head=newpcb;
		newpcb->next=newpcb;
	}
	else
	{
		newpcb->next=current_pcb->next;
		current_pcb->next = newpcb;
	}
	current_pcb=newpcb;
}



void start_sched()
{
	useless_pcb = (struct pcb_s *) AllocateMemory(sizeof(struct pcb_s));
	useless_pcb->stack_begin=AllocateMemory(STACK_SIZE); // garde en memoire le debut de la pile pour la suppresion
	useless_pcb->sp = useless_pcb->stack_begin + STACK_SIZE - 1;
	useless_pcb->next=head;
	current_pcb=useless_pcb;
	
	init_hw();
	ENABLE_IRQ();
	
	while(1){
	}

}

void sched()
{
	while(current_pcb->next->etat == TERMINATED)
	{
		FreeAllocatedMemory((uint32_t*)current_pcb->next->sp);
		FreeAllocatedMemory((uint32_t*)current_pcb->next->stack_begin);
		if(current_pcb->next == head)
		{
			head=current_pcb->next->next;
		}
		current_pcb->next= current_pcb->next->next;
		}
		next_running=current_pcb->next;
	}
	
void start_current_process()
{
	
	current_pcb->etat= READY;
	current_pcb->f();
	current_pcb->etat= TERMINATED;
	ctx_switch();

}
