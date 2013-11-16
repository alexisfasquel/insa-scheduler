#define N 100

#include "process.h"
#include "dispatcher.h"
#include "sched.h"
#include "semaphore.h"
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
	int cptA = 0;
	
	
	mtx_lock(mtx);
	shared_cpt++;
	ctx_switch();
	mtx_unlock(mtx);
	ctx_switch();
}

void
funcB()
{
	int cptB = 1;

	mtx_lock(mtx);
	shared_cpt++;
	mtx_unlock(mtx);
}

//------------------------------------------------------------------------
int
notmain ( void )
{
	//On cree les deux processus
	create_process(STACK_SIZE,funcA, NULL);
	create_process(STACK_SIZE,funcB, NULL);	
	
	//initialise le sémaphore
	mtx_init(mtx);
	
	//On lance l'ordonnanceur	
	start_sched();
	
	/* Pas atteignable vues nos 2 fonctions */
	return(0);
}
