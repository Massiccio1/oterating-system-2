// Per le funzioni di lettura e scrittura da stdin/stdout
#include <stdio.h>

// Gestione dei thread
#include <pthread.h>

// open
#include <fcntl.h>

// read, close
#include <unistd.h>

// Costanti del programma, fornite nella consegna:
// Nome del file contenente gli interi da leggere in memoria
#define NOME_FILE "/dev/random"
// Numero di interi da leggere
#define DIMENSIONE_VETTORE 10000
// Numero di thread fra i quali suddividere il calcolo della sommma
#define NUMERO_THREAD 10
// Numero di elementi del vettore da assegnare a ciascun thread
#define ELEMENTI_PER_THREAD (DIMENSIONE_VETTORE/NUMERO_THREAD)

// Parametri per ogni thread
struct parametri {
	// Mutex per modificare il totale alla fine del calcolo della somma parziale
	pthread_mutex_t *mutex;
	// Puntatore all'inizio della sottosequenza di interi da sommare
	unsigned int *sottoarray;
	// Numero di interi da sommare
	int lunghezza;
	// Puntatore alla variabile contenente il totale, su cui accumulare il risultato della somma parziale
	int *totale;
};

// Funzione del thread. Dati i parametri visti sopra, la funzione esegue in locale la somma parziale della sequanza di interi
// individuata dal puntatore "sottoarray" e da "lunghezza", poi somma il risultato parziale al totale, accedendovi in modo esclusivo
void *somma(void *p) {
	// Traduce il puntatore void* in puntatore alla struct parametri
	struct parametri *par = p;

	// Inizializza la somma parziale
	unsigned int parziale = 0;
	// Calcola la somma parziale
	int i;
	for (i = 0; i < par->lunghezza; i++) {
		parziale += par->sottoarray[i];
	}

	// Somma il parziale alla variabile condivisa "totale" accedendo tramite mutex
	pthread_mutex_lock(par->mutex);
	*par->totale += parziale;
	pthread_mutex_unlock(par->mutex);

	// Termina
	return NULL;
}

int main () {

	printf("Lettura di %d numeri dal file %s\n", DIMENSIONE_VETTORE, NOME_FILE);
	int fd = open(NOME_FILE, O_RDONLY);
	unsigned int numeri[DIMENSIONE_VETTORE];
	read(fd, numeri, sizeof(numeri)); // oppure DIMENSIONE_VETTORE * sizeof(unsigned int)
	close(fd);

	puts("Creazione del mutex");
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);

	puts("Avvio dei thread");
	// Variabile nella quale i thread accumuleranno il totale
	unsigned int totale = 0;
	// Per ogni thread dobbiamo creare un diverso struct parametri e ricordare il thread ID
	struct parametri par[NUMERO_THREAD];
	pthread_t thread[NUMERO_THREAD];
	int i;
	for (i = 0; i < NUMERO_THREAD; i++) {
		// impostiamo i parametri dell'i-esimo thread
		par[i].mutex = &mutex; // Il mutex è lo stesso per tutti, creato prima
		par[i].sottoarray = &numeri[i * ELEMENTI_PER_THREAD]; // Il thread i deve sommare 1000 elementi dell'array a partire da i*1000
		par[i].lunghezza = ELEMENTI_PER_THREAD;
		par[i].totale = &totale; // alla fine il thread deve aggiungere la somma parziale al totale
		printf("Avvio il thread %d\n", i);
		pthread_create(&thread[i], NULL, somma, &par[i]);
	}

	puts("Attesa della conclusione dei thread");
	// Ora attendiamo che i thread si concludano. Alla fine, ciascuno avrà sommato il proprio parziale al totale.
	for (i = 0; i < NUMERO_THREAD; i++) {
		void *valore;
		pthread_join(thread[i], &valore);
		printf("Thread %d concluso.\n", i);
	}

	printf("Somma totale: %u\n", totale);

	puts("Ricalcolo la somma dello stesso vettore nel thread principale");
	// Nuovo accumulatore del totale
	unsigned int totale2 = 0;
	// nuovi parametri: passiamo l'intero vettore numeri di 10000 elementi
	struct parametri par1 = { &mutex, numeri, DIMENSIONE_VETTORE, &totale2 };
	// Invochiamo direttamente la funzione, senza il bisogno di avviare un unovo thread
	somma(&par1);

	printf("Somma sequenziale: %u\n", totale2);

	// Verifichiamo se il risultato dell'operazione suddivisa fra 10 thread paralleli coincide col risultato sequenziale
	if (totale != totale2) {
		puts("I due totali sono diversi.");
		// Se i totali sono diversi, produce un valore di uscita non nullo per segnalare il fallimento
		return 1;
	}

	// Altrimenti, successo:
	puts("I due totali sono uguali.");
	return 0;
}
