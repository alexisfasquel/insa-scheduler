#include "process.h"
#include "dispatcher.h"

#define STACK_SIZE 128

struct ctx_s ctx_A;
struct ctx_s ctx_B;

void
funcA()
{
  int cptA = 0;

  while ( 1 ) {
    cptA ++;
    switch_to(&ctx_B);
  }
}

void
funcB()
{
  int cptB = 1;

  while ( 1 ) {
    cptB += 2 ;
    switch_to(&ctx_A);
  }
}

//------------------------------------------------------------------------
int
notmain ( void )
{
  init_ctx(&ctx_A, funcA, STACK_SIZE);
  init_ctx(&ctx_B, funcB, STACK_SIZE);

  switch_to(& ctx_A);

  /* Pas atteignable vues nos 2 fonctions */
  return(0);
}
