// fprintf
#include <stdio.h>
// atof
#include <stdlib.h>
// sleep
#include <unistd.h>
// pthread_*
#include <pthread.h>

// prototipi dileggi_float, thread_ascolto, stampa_statistiche
// definizione di struct parametri_thread
#include "funzioni_genitore.h"

// Lettura di un float espresso come stringa dal descrittore in input fd
float leggi_float(int fd) {
    char buffer[100];
    int l = read(fd, buffer, 100);
    // assumiamo che la stringa letta termini con un a capo, lo sostituiamo con un terminatore.
    buffer[l-1] = 0;
    return atof(buffer);
}

void *thread_ascolto(void *p) {
    struct parametri_thread *par = (struct parametri_thread*)p;
    while (1) {
        // lettura del punteggio dalla pipe
        float punteggio = leggi_float(par->fd_ascolto);
        // Accumulo del punteggio nelle variabili condivise
        pthread_mutex_lock(par->mutex);
        *par->punteggio_parziale += punteggio;
        *par->punteggio_totale += punteggio;
        pthread_mutex_unlock(par->mutex);
    }
}

void stampa_statistiche(pthread_mutex_t *mutex, pid_t pids[], float parziali[], float *totale) {
    while (1) {
        int i;
        pthread_mutex_lock(mutex);
        for (i = 0; i < N_PROCESSI; i++) {
            fprintf(stderr, "\t%d: %f\n", pids[i], parziali[i]);
        }
        fprintf(stderr, "\tTotale: %f\n", *totale);
        pthread_mutex_unlock(mutex);
        sleep(5);
    }
}

