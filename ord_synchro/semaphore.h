//Struture semaphore
struct sem_s 
{
	
	int compteur;
	struct pcb_s* blocked_pcb;
};

//Struture mutex
struct mtx_s
{
	int compteur;
	struct pcb_s* blocked_pcb;
	struct pcb_s* owner_pcb;
};

void sem_init(struct sem_s* sem, unsigned int val); //Initialise le sémaphore
void sem_up(struct sem_s* sem); //Ajoute un ticket au sémaphore et débloque un processus bloqué (si il y en a)
void sem_down(struct sem_s* sem); //Retire un ticket au sémaphore et bloque le processus appelant si il n'y avait pas assez de ticket

void mtx_init(struct mtx_s* mutex);
void mtx_lock(struct mtx_s* mutex);
void mtx_unlock(struct mtx_s* mutex);
