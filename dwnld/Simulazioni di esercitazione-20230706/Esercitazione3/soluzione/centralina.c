/*
centralina.c
Libreria per la gestione della centralina
*/

// standard output
#include <stdio.h>
// malloc
#include <stdlib.h>
// read
#include <unistd.h>
// gestione dei thread
#include <pthread.h>

// Prototipo della funzione di ingresso a questa libreria
#include "centralina.h"
// Valori dello stato della levetta dell'interruttore, condivisi con la centralina
#include "levetta.h"

enum stato_lampadina {
	SPENTA,
	ACCESA
};

// Variabili globali, per evitare la proliferazione di struct, inizializzate dalla funzione di ingresso alla libreria
// Le definiamo "static" per renderle inaccessibili al di fuori della libreria.

// Vettore dei descrittori di lettura delle pipe
int *descrittori_lettura;
// Numero di elementi del vettore / interruttori
int numero_interruttori;
// Vettore con lo stato di ciascun interruttore
enum posizione_levetta *stato_interruttori;
// Stato attuale della lampadina
enum stato_lampadina stato;
// Mutex per il ricalcolo dello stato della lampadina
pthread_mutex_t mutex;

// Date le posizioni delle levette degli interruttori, determina lo stato della lampadina e stampalo
void determina_stato_lampadina() {
	// Accesso esclusivo per evitare che un altro thread modifichi lo stato in contemporanea
	pthread_mutex_lock(&mutex);
	// Conteggio del numero di interruttori con la levetta in posizione alta
	int numero_alti = 0;
	int i;
	for (i = 0; i < numero_interruttori; i++)
		if (stato_interruttori[i] == ALTO)
			numero_alti++;
	// Determinazione del nuovo stato sulla base della parità del numero di levette alzate
	enum stato_lampadina nuovo_stato = numero_alti % 2 == 0 ? ACCESA : SPENTA;
	if (nuovo_stato != stato) {
		stato = nuovo_stato;
		// Notifica in caso di cambiamenti
		printf("Lampadina %s.\n", stato == ACCESA ? "accesa" : "spenta");
	}
	// Uscita dalla sezione critica
	pthread_mutex_unlock(&mutex);
}

// Funzione principale del thread
void *gestione_messaggi(void *p) {
	// Il parametro punta a un intero contenente l'indice del thread
	int indice = *(int*)p;

	// Ripeti all'infinito
	while (1) {
		// Attendi la ricezione di un messaggio e memorizzalo nello stato dell'interruttore corrispondente all'indice
		read(descrittori_lettura[indice], &stato_interruttori[indice], sizeof(enum posizione_levetta));
		// se necessario, modifica lo stato di accensione della lampadina
		determina_stato_lampadina();
	}

	// Non si dovrebbe mai arrivare a questo return
	return NULL;
} 

// Funzione di ingresso della librerie
void centralina (int fd[], int n) {
	// Inizializza le variabili globali
	descrittori_lettura = fd;
	numero_interruttori = n;
	// Alloca il vettore per contenere lo stato di tutti gli interruttori
	stato_interruttori = (enum posizione_levetta*)malloc(numero_interruttori * sizeof(enum posizione_levetta));
	// Inizializza il mutex per l'accesso alla sezione critica per la gestione della lampadina (attributi di default)
	pthread_mutex_init(&mutex, NULL);

	// Creazione dei thread
	// Schiera di variabili contenenti gli indici dei thread, da passare come parametri
	int *indici = (int*)malloc(numero_interruttori * sizeof(int));
	int i;
	for (i = 0; i < numero_interruttori; i++) {
		// Per ogni thread dobbiamo definire una variabile per ospitare il thread id, anche se non lo useremo
		pthread_t t;
		// Imposta il parametro da passare al thread
		indici[i] = i;
		if (i < numero_interruttori - 1) {
			// Tutti i thread vanno creati, ad esclusione dell'ultimo
			pthread_create(&t, NULL, gestione_messaggi, &indici[i]);
		} else {
			// L'ultimo thread non ha bisogno di esserre creato, pasta usare quello principale chiamando direttamente la funzione.
			gestione_messaggi(&indici[i]);
		}
	}
}
