// printf, fprintf
#include <stdio.h>

// read
#include <unistd.h>

// open
#include <fcntl.h>

// L'inclusione dell'header non è strettamente necessaria, ma contiene il prototipo della funzione qui definita.
// L'inclusione garantisce che il compilatore genererà un warning se il prototipo non è compatibile con
// la definizione della funzione.
#include "giocatore.h"

// Loop infinito: generazione e stampa dei punteggi ottenuti
void giocatore() {
    // Apre in lettura il "file" che genera sequenze casuali di byte
    int random_fd = open("/dev/random", O_RDONLY);
    // Loop infinito
    while (1) {
        // variabile in cui leggere il valore casuale
        unsigned int valore_intero;
        // lettura dei 4 byte di valore_intero dal file
        read(random_fd, &valore_intero, sizeof(valore_intero));
        // Condizione per "vincere"
        if (valore_intero < 10000 && valore_intero > 0) {
            // Calcolo del punteggio come reciproco del valore letto
            float punteggio = 1.0f / valore_intero;
            // Stampa del pid e del punteggio sullo standard error
            fprintf(stderr, "%d: %f\n", getpid(), punteggio);
            // Stampa del punteggio sullo standard output
            printf("%f\n", punteggio);
            // per evitare il buffering, forziamo l'emissione dello standard output subito dopo la stampa
            fflush(stdout);
        }
    }
}
