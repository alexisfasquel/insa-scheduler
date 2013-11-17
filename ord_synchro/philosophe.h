#ifndef __PHILOSOPHE_H
#define __PHILOSOPHE_H

#include "semaphore.h"

struct mtx_s* mtx_protection;
struct mtx_s* fourchette1;
struct mtx_s* fourchette2;
struct mtx_s* fourchette3;
struct mtx_s* fourchette4;
struct mtx_s* fourchette5;

void philo_init(); // initialise nos mutex
void philosophe(int i); //Actions du philosophe i
void prendreFourchette(int i);
void poserFourchette(int i);
void penser();
void manger();




#endif
