/*
main.c
Programma principale
Sintassi:
./main n
dove n è un intero che definisce il numero di interruttori
*/

// fork, pipe, close
#include <unistd.h>
// malloc
#include <stdlib.h>

// Funzione per la gestione dell'interruttore, da invocare nei processi figlio
#include "interruttore.h"
// Funzione per la gestione della centralina, da invocare alla fine della creazione dei figli
// nel processo genitore
#include "centralina.h"

int main (int argc, char *argv[]) {
	// Leggi il numero di interruttori dal primo argomento della linea di comando
	int numero_interruttori = atoi(argv[1]);
	// Alloca spazio per il vettore dei descrittori in lettura delle pipe, da passare alla centrallina
	int *descrittori_lettura = (int*)malloc(numero_interruttori * sizeof(int));

	// Creazione dei figli (gestione interruttori)
	int i;
	for (i = 0; i < numero_interruttori; i++) {
		// Crea la pipe per la comunicazione tra l'i-esimo interruttore e la centralina
		int fd[2];
		pipe(fd);
		// Crea il nuovo processo
		if (fork()) {
			// Processo genitore: non serve il ramo di scrittura 
			close(fd[1]);
			// Ricorda il descrittore di lettura nel vettore per passarlo alla centralina
			descrittori_lettura[i] = fd[0];
		} else {
			// Processo figlio: non serve il ramo di lettura della pipe
			close(fd[0]);
			// Avvia la gestione dell'interruttore (loop infinito)
			interruttore(fd[1]);
			// Non si dovrebbe mai arrivare a questo return
			return 1;
		}
	}
	// Creazione dei processi figlio terminata: avvia la centralina nel processo principale
	centralina(descrittori_lettura, numero_interruttori);
	// Non si dovrebbe mai arrivare a questo return
	return 0;
}
