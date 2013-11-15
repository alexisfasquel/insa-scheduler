#define N 100

#include "process.h"
#include "dispatcher.h"
#include "sched.h"

#define STACK_SIZE 128
#define NULL 0

struct sem_s* sem;


void
funcA()
{
	int cptA = 0;
	sem_down(sem); //Retire un ticket, et est donc bloqué (vu que sem initialisé à 0)
	
	while ( 1 ) 
	{
		cptA ++; //tourne en boucle en attendant interruption qui change de contexte
	}
}

void
funcB()
{
	int cptB = 1;
	sem_up(sem); //Ajoute un ticket et débloque pcbA
	while ( 1 ) 
	{
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
	
	//initialise le sémaphore
	sem_init( sem, 0);
	
	//On lance l'ordonnanceur	
	start_sched();
	
	/* Pas atteignable vues nos 2 fonctions */
	return(0);
}
