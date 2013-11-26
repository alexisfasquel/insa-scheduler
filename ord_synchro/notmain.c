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

struct mtx_s* mtx;

void
funcA()
{

	mtx_lock(mtx);
	yield(0);
	mtx_unlock(mtx);
	
	while(1)
	{
	}
}

void
funcB()
{
	int i =0;
	mtx_lock(mtx);
	i++;
	while(1)
	{
	}
}


//------------------------------------------------------------------------
int
notmain ( void )
{
	//On cree les deux processus
	/*create_process(STACK_SIZE,funcA, NULL);
	create_process(STACK_SIZE,funcB, NULL);	*/


	//On cree nos 5 philosophes
	create_process(STACK_SIZE,philosophe, (void *) 1);
	create_process(STACK_SIZE,philosophe, (void *) 2);
	create_process(STACK_SIZE,philosophe, (void *) 3);
	create_process(STACK_SIZE,philosophe, (void *) 4);
	create_process(STACK_SIZE,philosophe, (void *) 5);
	
	//On initialise nos mutex
	philo_init();
	
	//initialise le mtx
	//mtx_init(mtx);

	
	//On lance l'ordonnanceur	
	start_sched();
	
	/* Pas atteignable vues nos 2 fonctions */
	return(0);
}
