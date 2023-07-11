/*
Esempio di uso di thread e mutex in C, libreria di sistema libpthread.

Crea 10 thread, ciascuno stampa 2000 righe a 10 a 10. Ogni gruppo di 10 righe è stampato senza interruzioni grazie a un mutex.
*/

#include <pthread.h>
#include <stdio.h>

/* Numero di thread da cerare */
#define N_THREADS 10

/* Struttura contenente i parametri da comunicare alla funzione del thread
all'avvio*/
struct parametri {
	char c; /* Carattere da stampare a inizio riga, per distinguere gli output */
	pthread_mutex_t *mutex; /* Puntatore al mutex che gestisce l'accesso alla sezione critica */
};

/* Funzione del thread */
void* prova(void *p) {
	struct parametri *par = (struct parametri*)p; /* Cast del parametro della funzione al tipo corretto */
	int i, j, k;
	for (i=0; i<20; i++) {
		pthread_mutex_lock(par->mutex);
		/* Sezione critica: stampa 10 linee senza interruzioni */
		for (j=0; j<10; j++) {
			for(k=0; k<1000; k++);
			printf("%c %d\n", par->c, i*10+j);
		}
		pthread_mutex_unlock(par->mutex);
		/* Sezione non critica: attendi 10000 iterazioni a vuoto */
		for(k=0; k<10000; k++);
	}
	return NULL;
}

int main() {
	/* creazione del mutex da condividere fra tutti i thread */
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);
	/* Ogni thread ha il suo struct di parametri */
	struct parametri p[N_THREADS];
	/* Vettore per memorizzare l'ID di ogni thread creato */
	pthread_t tid[N_THREADS];
	
	/* Creazione dei thread */
	int i;
	for (i=0; i<N_THREADS; i++) {
		/* inizializza lo struct con i parametri del thread i */
		p[i].c = 'A' + i; /* 'A', 'B', 'C',... */
		p[i].mutex = &mutex; /* Il mutex è lo stesso per tutti i thread */
		/* Avvia il thread i passando l'i-esimo struct di parametri e ricevendo l'i-esimo id */
		pthread_create(tid+i, NULL, prova, p+i);
	}
	
	/* Attesa della terminazione di ciascun thread */
	void *ritorno;
	for (i=0; i<N_THREADS; i++) {
		pthread_join(tid[i], &ritorno);
	}
	puts("Tutti i thread sono terminati.");
	pthread_mutex_destroy(&mutex);
	return 0;
}








