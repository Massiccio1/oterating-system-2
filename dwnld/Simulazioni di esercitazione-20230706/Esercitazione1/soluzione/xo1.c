// Per le funzioni di lettura e scrittura da stdin/stdout
#include <stdio.h>

// pid_t
#include <sys/types.h>

// fork, dup2, sleep
#include <unistd.h>

int main () {

	// Descrittori della pipe anonima
	int fd[2];

	// ID del processo figlio (restituito da fork() al genitore)
	pid_t id;

	// Creazione della pipe anonima
	pipe(fd);

	// Duplicazione del processo
	id = fork();
	if (id) {

		// Processo genitore

		// Serve solo il ramo scrivibile della pipe, chiudiamo l'altro
		close(fd[0]);
		// Duplica il ramo scrivibile della pipe sullo standard output
		dup2(fd[1], 1);

		// Ripeti all'infinito
		while(1) {
			// Attendi un secondo
			sleep(1);
			// Scrivi la stringa sullo standard output e assicurati che venga inviata
			puts("Ciao, mondo!");
			fflush(stdout);
		}
	} else {

		// Processo figlio

		// Stampa il PID
		printf("%d\n", getpid());

		// Serve solo il ramo leggibile della pipe, chiudiamo l'altro
		close(fd[1]);
		// Duplica il ramo leggibile della pipe sullo standard input
		dup2(fd[0], 0);

		// Ripeti all'infinito
		while(1) {
			char buffer;
			// Leggi un carattere dallo standard input
			buffer = getchar();
			// Converti la 'o' in 'x'
			if (buffer == 'o') buffer = 'x';
			// Stampa il carattere sullo standard output
			putchar(buffer);
		}
	}
	return 0;
}
