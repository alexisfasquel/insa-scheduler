#include "sched.h"
#include "dispatcher.h"
#include "process.h"
#include "hw.h"

#define STACK_SIZE 12800
#define NULL 0

struct pcb_s* head=NULL; //Tête de la liste des processus ready
struct pcb_s* current_pcb; // Processus courant

void create_process(int stack_size,func_t f, void* args)
//Cree un processus et le rajoute dans la liste des processus ready
{
	struct pcb_s* newpcb =  (struct pcb_s*)AllocateMemory(sizeof(struct pcb_s)); //Alloue la place nécessaire pour le processus
	init_pcb(newpcb, STACK_SIZE); //Alloue la place nécessaire pour la pile du processus
	newpcb->args=args;
	newpcb->etat = NEW;
	newpcb->f = (void*)f;
	if(head == NULL) //Si la liste des processus ready est vide
	{
		//Le nouveau pcb est en tête
		head=newpcb;
		newpcb->next=newpcb;
	}
	else
	{
		//Sinon on le rajoute à la liste
		newpcb->next=current_pcb->next;
		current_pcb->next = newpcb;
	}
	current_pcb=newpcb;
}



void start_sched()
{
	//On utilise un processus inutile au début pour qu'on ait un current_pcb avant de commencer
	useless_pcb = (struct pcb_s *) AllocateMemory(sizeof(struct pcb_s));
	useless_pcb->stack_begin=AllocateMemory(STACK_SIZE); 
	useless_pcb->sp = useless_pcb->stack_begin + STACK_SIZE - 1;
	useless_pcb->next=head;
	current_pcb=useless_pcb;
	
	init_hw(); // On initialise le timer
	ENABLE_IRQ(); // On autorise les interruptions (sinon elles ne fonctionnent pas)
	
	while(1) // On laisse du temps au prog pour déclencher les interruptions, sinon le prog se terminerait
	{
	}

}

void sched()
//Supprimer les processus terminés, et choisit le processus suivant
{
	while(current_pcb->next->etat == TERMINATED) //Si il y a des processus terminés dans la file, on les supprime
	{
		FreeAllocatedMemory((uint32_t*)current_pcb->next->sp);
		FreeAllocatedMemory((uint32_t*)current_pcb->next->stack_begin);
		if(current_pcb->next == head)
		{
			head=current_pcb->next->next;
		}
		current_pcb->next= current_pcb->next->next;
	}
	
	//Si le processus suivant dans la pile est ready, on le choisit comme étant le suivant à être exécuté
	next_running=current_pcb->next;}

void start_current_process()
{

	current_pcb->etat= READY;
	current_pcb->f();
	current_pcb->etat= TERMINATED;
	ctx_switch();

}
