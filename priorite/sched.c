#include "sched.h"
#include "dispatcher.h"
#include "process.h"
#include "hw.h"

#define STACK_SIZE 12800
#define NULL 0

struct pcb_s* head=NULL; //Tête de la liste des processus ready
struct pcb_s* current_pcb; // Processus courant
struct pcb_s* parcours; // Processus courant
int temps_horloge;
struct pcb_s* pcb_actuel;

void create_process(int stack_size,func_t f, void* args,int prioritepcb)
//Cree un processus et le rajoute dans la liste des processus ready
{
	struct pcb_s* newpcb =  (struct pcb_s*)malloc_alloc(sizeof(struct pcb_s)); //Alloue la place nécessaire pour le processus
	init_pcb(newpcb, STACK_SIZE,prioritepcb); //Alloue la place nécessaire pour la pile du processus
	newpcb->args=args;
	newpcb->etat = NEW;
	newpcb->f = (void*)f;
	if(head == NULL) //Si la liste des processus ready est vide
	{
		//Le nouveau pcb est en tête
		head=newpcb;
		newpcb->next=newpcb;
		newpcb->prev=newpcb;
	}
	else
	{
		parcours = head;
		//Sinon on le rajoute à la liste
		//On prend en fonction la priorite du pcb pour l'ajouter au bon endroit
		while((newpcb->priorite <= parcours->priorite) && (parcours->next != head))
		{
			parcours=parcours->next;
		}
		//mettre ds un si
			
		if(newpcb->priorite > parcours->priorite){
			newpcb->prev=parcours->prev;
			parcours->prev = newpcb;
			newpcb->next=parcours;
		}else{
			newpcb->next=parcours->next;
			parcours->next = newpcb;
			newpcb->prev = parcours;
		}


		if(parcours == head && newpcb->priorite > parcours->priorite ){
			head=newpcb;
		}
		

	}
	
}








void start_sched()
{
	//On utilise un processus inutile au début pour qu'on ait un current_pcb avant de commencer
	useless_pcb = (struct pcb_s *) malloc_alloc(sizeof(struct pcb_s));
	useless_pcb->stack_begin=(uint32_t*) malloc_alloc(STACK_SIZE); 
	useless_pcb->sp = useless_pcb->stack_begin + (STACK_SIZE / sizeof(uint32_t)) - 1;
	useless_pcb->next=head;
	useless_pcb->etat=NEW;
	useless_pcb->priorite = 0;
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
	checkwaiting();
	pcb_actuel = current_pcb; 
	while(current_pcb->next->etat == TERMINATED) //Si il y a des processus terminés dans la file, on les supprime
	{
		malloc_free((uint32_t*)current_pcb->next->sp);
		malloc_free((uint32_t*)current_pcb->next->stack_begin);
		if(current_pcb->next == head)
		{
			head=current_pcb->next->next;
		}
		current_pcb->next= current_pcb->next->next;
	}
	
	//Si le processus suivant dans la pile est ready, on le choisit comme étant le suivant à être exécuté
	if(current_pcb->etat == TERMINATED){
		next_running=current_pcb->next;
		malloc_free((uint32_t*)current_pcb->sp);
		malloc_free((uint32_t*)current_pcb->stack_begin);
		if(current_pcb == head)
		{
			head=current_pcb->next;
		}
		current_pcb->prev->next= current_pcb->next;
	}
	else{

		if(current_pcb->etat == WAITING)
		{
			while(current_pcb->etat != NEW && current_pcb->etat != READY)
			{
				current_pcb=current_pcb->next;
			}
			next_running=current_pcb;
			return;

		}
		else
		{
			if(current_pcb->next->priorite < current_pcb->priorite){
				next_running=current_pcb;
				return;
			}
			else
			{
				while(current_pcb->next->priorite >= current_pcb->priorite && current_pcb->next->etat == WAITING ){
					current_pcb=current_pcb->next;
				}
				if(current_pcb->next->priorite < current_pcb->priorite){
					next_running=pcb_actuel;
				}else{
					next_running=current_pcb->next;
				}
			}
		}

	}
		
}




void start_current_process()
{

	current_pcb->etat= READY;
	current_pcb->f();
	current_pcb->etat= TERMINATED;
	ctx_switch();

}

void checkwaiting(){
//Verifie si un process a finit d'attendre
	parcours=head;
	while(parcours->next != head)
	{
		if(parcours->etat == WAITING && temps_horloge > parcours->temps_reveil){
			parcours->etat=READY;
		}
		parcours=parcours->next;
	}
}

void sleep(int number){
	
	current_pcb->temps_reveil=number+temps_horloge;
	current_pcb->etat=WAITING;
}
