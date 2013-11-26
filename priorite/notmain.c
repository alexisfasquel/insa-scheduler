#include "process.h"
#include "dispatcher.h"
#include "sched.h"
#include "malloc.h"
#include "hw.h"



#define STACK_SIZE 128
#define NULL 0

void
funcA()
{ 
	while(1)
	{
	    	led_on();
		//sleep(1);
	}
}

void
funcB()
{
	while(1)
	{
    		led_off();
    		//sleep(1);
	}
}
void

funcC(){
		volatile int a=0 ;
	while(1){
		a++;
	}
}

//------------------------------------------------------------------------
int
notmain ( void )
{


  	DISABLE_IRQ();
  	init_hw();
  	malloc_init( (void*)0x50000);

	//On cree les deux processus
	create_process(STACK_SIZE,funcA, NULL,2);
	create_process(STACK_SIZE,funcB, NULL,2);
	create_process(STACK_SIZE,funcC, NULL,1);
	
	//On lance l'ordonnanceur	
	start_sched();
	
	/* Pas atteignable vues nos 2 fonctions */
	return(0);
}
