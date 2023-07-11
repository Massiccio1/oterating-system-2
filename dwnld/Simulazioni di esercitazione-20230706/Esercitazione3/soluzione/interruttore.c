/*
interruttore.c
Funzioni per la gestione di un singolo interruttore
*/

// standard output
#include <stdio.h>
// write, getpid, pause
#include <unistd.h>
// signal, SIGUSR1/2
#include <signal.h>

// Prototipo della funzione di ingresso a questa libreria
#include "interruttore.h"
// Valori dello stato della levetta dell'interruttore, condivisi con la centralina
#include "levetta.h"

// Le variabili su cui questa libreria lavora devono essere globali in quanto gestite dalla funziona signal()
// Le definiamo "static" per renderle inaccessibili al di fuori della libreria.

// Stato corrente dell'interruttore
static enum posizione_levetta stato;
// Descrittore di scrittura della pipe di comunicazione con la centralina
static int descrittore_scrittura;

// Invia lo stato dell'interruttore alla centralina attraverso la pipe
static void invia_stato() {
	write(descrittore_scrittura, &stato, sizeof(stato));
}

// Gestore dei segnali: registra il nuovo stato della levetta e, se è cambiato, lo spedisce alla centralina
static void spostamento_levetta(int segnale) {
	// Determina il nuovo stato sulla base del segnale ricevuto (SIGUSR1->ALTO, SIGUSR2->BASSO)
	enum posizione_levetta nuovo_stato =
		segnale == SIGUSR1 ? ALTO
		: segnale == SIGUSR2 ? BASSO
		: stato; // per default lo stato non cambia
	if (nuovo_stato != stato) {
		// Se lo stato è cambiato, stampa la notifica
		printf("Nuovo stato dell'interruttore %u: %s.\n", getpid(), stato == ALTO ? "alto" : "basso");
		// Registra il nuovo stato
		stato = nuovo_stato;
		// Invialo alla centralina
		invia_stato();
	}
}

// Punto di ingresso della libreria
void interruttore(int fd) {
	// Stampa il PID del processo per consentire l'invio dei segnali
	printf("Interruttore %u\n", getpid());

	// Inizializza le variabili globali, in modo che siano condivisibili con le altre funzioni della libreria
	descrittore_scrittura = fd;
	// Inizia con la leva in posizione bassa e inviala alla centralina
	stato = BASSO;
	invia_stato();

	// Registra il gestore per i due segnali
	signal(SIGUSR1, spostamento_levetta);
	signal(SIGUSR2, spostamento_levetta);

	// Ciclo infinito, pause() si risveglia ad ogni segnale ricevuto 
	while (1) pause();
}
