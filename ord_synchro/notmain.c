#define N 100

#include "process.h"
#include "dispatcher.h"
#include "sched.h"
#include "semaphore.h"
#include "philosophe.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define STACK_SIZE 128
#define NULL 0

struct sem_s* sem;
struct mtx_s* mtx;
uint32_t shared_cpt;


void
funcA()
{
	mtx_lock(mtx);
	shared_cpt++;
	ctx_switch();
	mtx_unlock(mtx);
	ctx_switch();
}

void
funcB()
{
	mtx_lock(mtx);
	shared_cpt++;
	mtx_unlock(mtx);
}

//------------------------------------------------------------------------
int
notmain ( void )
{
	//On cree les deux processus
	//create_process(STACK_SIZE,funcA, NULL);
	//create_process(STACK_SIZE,funcB, NULL);	
	
	//On cree nos 5 philosophes
	create_process(STACK_SIZE,philosophe, (int) 1);
	create_process(STACK_SIZE,philosophe, (int) 2);
	create_process(STACK_SIZE,philosophe, (int) 3);
	create_process(STACK_SIZE,philosophe, (int) 4);
	create_process(STACK_SIZE,philosophe, (int) 5);
	
	//On initialise nos mutex
	philo_init();
	
	//initialise le sémaphore
	//mtx_init(mtx);
	
	//On lance l'ordonnanceur	
	start_sched();
	
	/* Pas atteignable vues nos 2 fonctions */
	return(0);
}
