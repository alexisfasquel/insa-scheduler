#include "process.h"
#include "dispatcher.h"
#include "sched.h"

#define STACK_SIZE 128
#define NULL 0

struct pcb_s pcb_A;
struct pcb_s pcb_B;
struct pcb_s pcb_init;

void
funcA()
{
  int cptA = 0;

  while ( 1 ) {
    cptA ++;
    yield();
  }
}

void
funcB()
{
  int cptB = 1;

  while ( 1 ) {
    cptB += 2 ;
    yield();
  }
}

//------------------------------------------------------------------------
int
notmain ( void )
{
  create_process(STACK_SIZE,funcA, NULL);
  create_process(STACK_SIZE,funcB, NULL);

  current_pcb=&pcb_init;

  start_sched();

  /* Pas atteignable vues nos 2 fonctions */
  return(0);
}
