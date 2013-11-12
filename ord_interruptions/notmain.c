#include "process.h"
#include "dispatcher.h"
#include "sched.h"

#define STACK_SIZE 128
#define NULL 0

void
funcA()
{
	int cptA = 0;
	
	while ( 1 ) {
		cptA ++; //tourne en boucle en attendant interruption qui change de contexte
	}
}

void
funcB()
{
	int cptB = 1;
	
	while ( 1 ) {
		cptB += 2 ;//tourne en boucle en attendant interruption qui change de contexte
	}
}

//------------------------------------------------------------------------
int
notmain ( void )
{
	//On cree les deux processus
	create_process(STACK_SIZE,funcA, NULL);
	create_process(STACK_SIZE,funcB, NULL);
	
	//On lance l'ordonnanceur	
	start_sched();
	
	/* Pas atteignable vues nos 2 fonctions */
	return(0);
}
