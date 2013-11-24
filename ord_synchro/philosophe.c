#include "semaphore.h"
#include "sched.h"
#include "philosophe.h"
#include "dispatcher.h"

void philo_init() //Initialise nos mutex
{
	mtx_init(mtx_protection);
	mtx_init(fourchette1);
	mtx_init(fourchette2);
	mtx_init(fourchette3);
	mtx_init(fourchette4);
	mtx_init(fourchette5);

}

void prendreFourchette(int i)
{
	switch (i) 
	{
		case 0 : //Si i=0, on appelle la fourchette qui est en fait celle de numéro max
			 mtx_lock(fourchette5);			
			 break;		
		case 1 : 
			 mtx_lock(fourchette1);			
			 break;	
		case 2 : 
			 mtx_lock(fourchette2);			
			 break;	
		case 3 : 
			 mtx_lock(fourchette3);			
			 break;	
		case 4 : 
			 mtx_lock(fourchette4);			
			 break;	
		case 5 : 
			 mtx_lock(fourchette5);			
			 break;		
	}

}

void poserFourchette(int i)
{
	switch (i) 
	{
		case 0 : //Si i=0, on appelle la fourchette qui est en fait celle de numéro max
			 mtx_unlock(fourchette5);			
			 break;		
		case 1 : 
			 mtx_unlock(fourchette1);			
			 break;	
		case 2 : 
			 mtx_unlock(fourchette2);			
			 break;	
		case 3 : 
			 mtx_unlock(fourchette3);			
			 break;	
		case 4 : 
			 mtx_unlock(fourchette4);			
			 break;	
		case 5 : 
			 mtx_unlock(fourchette5);			
			 break;		
	}
}

void penser()
{
	int i;

	for(i=0; i<1000000 ; i++)
	{
	}
}

void manger()
{
	int i;

	for(i=0; i<1000000 ; i++)
	{
	}
}


void philosophe(void* j) //Actions du philosophe i
{
	int i = (int) j;
	for ( ; ; )
	{
		penser();
		mtx_lock(mtx_protection); //protection du code pour éviter les interblocages
		prendreFourchette(i); 
		prendreFourchette( i-1);
		manger();
		poserFourchette(i);
		poserFourchette(i-1);
		mtx_unlock(mtx_protection); //fin de la protection
	}
}

