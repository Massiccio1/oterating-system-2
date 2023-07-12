// Stampa sui canali standard
#include <stdio.h>

// atoi()
#include <stdlib.h>

// strlen()
#include <string.h>

// open(), costanti O_*
#include <fcntl.h>

// write(), read(), close(), fork(), sleep()
#include <unistd.h>

// signal(), costanti SIG*
#include <signal.h>

// Variabili globali, per comodità di accesso da parte dei gestori dei segnali.
//
// contatore delle righe stampate, da preincrementare a ogni stampa
int contatore = 0;
// Intervallo fra una stampa e la successiva
int ritardo = 1;

// Funzione di utilità: scrive un intero nella pipe codificandolo come stringa terminata da un a capo.
// Come da specifiche, il processo può assumere di essere l'unico ad usarla in questo momento.
// NOTA BENE: la pipe deve essere aperta e chiusa ogni volta, perché altri processi potrebbero utilizzarla in seguito.
void scrivi_pipe(int n) {

    // Utilizzando le primitive di basso livello:

    // buffer di caratteri per la scrittura di stringhe nella pipe
    char buffer[100];
    // inserire il valore come stringa nel buffer, terminato con un a capo
    sprintf(buffer, "%d\n", n);
    // apertura della pipe in scrittura
    int fd = open("pipe", O_WRONLY);
    // scrittura del buffer per il numero di caratteri contenuti
    write(fd, buffer, strlen(buffer));
    // chiusura della pipe
    close(fd);

    // In alternativa, per utilizzare gli stream bufferizzati
    // commentare le righe sopra e scommentare il codice sottostante:

    // // apertura della pipe in scrittura
    // FILE *f = fopen("pipe", "w");
    // // scrittura dell'argomento intero seguito da un a capo
    // fprintf(f, "%d\n", n);
    // // chiusura delo stream, con conseguente flush del contenuto
    // fclose(f);
}

// Funzione di utilità: legge un innero, codificato come stringa terminata da un a capo, dalla pipe.
// Come da specifiche, il processo può assumere di essere l'unico ad usare la pipe in questo momento.
// NOTA BENE: la pipe deve essere aperta e chiusa ogni volta, perché altri processi potrebbero utilizzarla in seguito.
int leggi_pipe() {

    // Utilizzando le primitive di basso livello:

    // buffer di caratteri per la lettura di stringhe dalla pipe
    char buffer[100];
    // apertura della pipe in lettura
    int fd = open("pipe", O_RDONLY);
    // lettura del contenuto della pipe (l byte)
    int l = read(fd, buffer, 100);
    // chiusura della pipe
    close(fd);
    // sostituzione dell'ultimo carattere letto con il terminatore di stringa
    buffer[l-1] = '\0';
    // Restituiamo il contenuto del buffer decodificato come intero
    return atoi(buffer);

    // In alternativa, per utilizzare gli stream bufferizzati
    // commentare le righe sopra e scommentare il codice sottostante:

    // // Variabile per contenere il risultato
    // int n;
    // // apertura dello stream in lettura
    // FILE *f = fopen("pipe", "r");
    // // lettura di un intero. Per sicurezza leggiamo anche l'a capo per evitare che rimanga nella pipe bloccando le scritture successive
    // // e osserviamo che la variabile n va passata per indirizzo, perché la funzione deve modificarla
    // fscanf(f, "%d\n", &n);
    // // chiusura dello stream
    // fclose(f);
    // // restituzione dell'intero letto
    // return n;
}

///////////////////////////////////////////
//
// Funzioni di gestione dei segnali

// Funzione da invocare alla ricezione di SIGUSR1
void invia_contatore() {
    // inviare il contatore attraverso la pipe
    scrivi_pipe(contatore);
}

// funzione da invocare alla ricezione di SIGUSR2
void ricevi_ritardo() {
    // lettura della pipe che contiene il nuovo ritardo
    ritardo = leggi_pipe();
}

// Funzione da invocare alla ricezione di SIGALRM
void sdoppia() {
    // Sdoppia il processo
    if (!fork()) {
        // il figlio deve inviare il pid attraverso la pipe
        scrivi_pipe(getpid());
    }
    // dopo l'invocazione della fork(), il genitore non deve fare nulla.
}

//////////////////////////////////////////////
//
// Programma principale

int main() {
    // invio del pid attraverso la pipe
    scrivi_pipe(getpid());

    // registrazione dei tre gestori dei segnali
    signal(SIGUSR1, invia_contatore);
    signal(SIGUSR2, ricevi_ritardo);
    signal(SIGALRM, sdoppia);

    // ciclo infinito
    for (;;) {
        // stampa in stderr la stringa "<pid> <contatore>\n" e incrementa il contatore
        fprintf(stderr, "%d %d\n", getpid(), ++contatore);
        // attendi il ritardo previsto in secondi
        sleep(ritardo);
    }
    return 0;
}
