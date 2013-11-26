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
	 volatile int i = 0;
  
  while (1) {
    led_on();
   while ( i++ < 2000000);
    i = 0;
  }


}

void
funcB()
{
	volatile int i = 0;

  while (1) {
    led_off();
    while ( i++ < 2000000);
    i = 0;
  }


	
}

//------------------------------------------------------------------------
int
start_kernel ( void )
{


  	DISABLE_IRQ();
  	init_hw();
  	malloc_init( (void*)0x50000);

	//On cree les deux processus
	create_process(STACK_SIZE,funcA, NULL);
	create_process(STACK_SIZE,funcB, NULL);
	
	//On lance l'ordonnanceur	
	start_sched();
	
	/* Pas atteignable vues nos 2 fonctions */
	return(0);
}
