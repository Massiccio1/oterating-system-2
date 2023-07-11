#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

// Per la gestione dei segnali
#include <signal.h>

// 1: converti 'o' in 'x'; 0: non convertire
int conversione = 1; // per default, converti

// Gestore dei segnali ricevuti (utile per il processo figlio)
void modifica_comportamento(int segnale) {
    if (segnale == SIGUSR1)
        conversione = 0; // SIGUSR1 inibisce la conversione o->x
    else if (segnale == SIGUSR2)
        conversione = 1; // SIGUSR2 la ripristina
}

int main () {
	int fd[2];
	pid_t id;

	pipe(fd);

	id = fork();
	if (id) {//padre

		close(fd[0]);
		dup2(fd[1], 1);

		while(1) {
			sleep(1);
			puts("Ciao, mondo!");
			fflush(stdout);
		}
	} else { // figlio

        // Gestisci i segnali SIGUSR1 e SIGUSR2
        signal(SIGUSR1, modifica_comportamento);
        signal(SIGUSR2, modifica_comportamento);

		printf("%d\n", getpid());

		close(fd[1]);
		dup2(fd[0], 0);

		while(1) {
			char buffer;
			buffer = getchar();

            // Converti solo se conversione==1
			if (conversione && buffer == 'o') buffer = 'x';
			putchar(buffer);
		}
	}
	return 0;
}
