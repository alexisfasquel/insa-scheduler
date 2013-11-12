#define N 100

#include "process.h"
#include "dispatcher.h"
#include "sched.h"

#define STACK_SIZE 128
#define NULL 0

struct pcb_s pcb_A;
struct pcb_s pcb_B;
struct pcb_s pcb_init;

struct sem_s* sem;


void
funcA()
{
  int cptA = 0;
  sem_down(sem);

  while ( 1 ) {
    cptA ++;
  }
}

void
funcB()
{
  int cptB = 1;
  sem_up(sem);
  while ( 1 ) {
    cptB += 2 ;
  }
}

//------------------------------------------------------------------------
int
notmain ( void )
{
  create_process(STACK_SIZE,funcA, NULL);
  create_process(STACK_SIZE,funcB, NULL);

  current_pcb=&pcb_init;

  
  sem_init( sem, 0);

  start_sched();

  /* Pas atteignable vues nos 2 fonctions */
  return(0);
}
