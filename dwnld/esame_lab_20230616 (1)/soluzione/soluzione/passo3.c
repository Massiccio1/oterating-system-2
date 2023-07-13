// pipe(), dup2(), close(), exec*()
#include <unistd.h>
// pthread_*()
#include <pthread.h>

// struct parametri_thread, thread_ascolto(), stampa_statistiche()
#include "funzioni_genitore.h"

// La funzione giocatore() non è invocata direttamente da passo3
// #include "giocatore.h"

int main () {
    // variabili da condividere con i thread
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    float punteggi_parziali[N_PROCESSI];
    float punteggio_totale = 0;
    struct parametri_thread par[N_PROCESSI];
    // vettore dei PID dei figli per la stampa delle statistiche
    pid_t pids[N_PROCESSI];
    // Creazione delle 10 pipe, dei processi giocatori e dei rispettivi thread di ascolto
    int i;
    for (i = 0; i < 10; i++) {
        int fd[2];
        int r = pipe(fd);
        int pid_figlio = fork();
        if (pid_figlio) {
            // Processo genitore
            // Raccogli i PID dei figli per stamparli nelle statistiche
            pids[i] = pid_figlio;
            // Raccolta die parametri per il thread i
            par[i].pid_figlio = pid_figlio;
            par[i].punteggio_parziale = &punteggi_parziali[i];
            punteggi_parziali[i] = 0;
            par[i].punteggio_totale = &punteggio_totale;
            par[i].mutex = &mutex;
            par[i].fd_ascolto = fd[0];
            // chiusura del lato in scrittura della pipe
            close(fd[1]);
            // avvio del thread di ascolto
            pthread_t tid;
            pthread_create(&tid, NULL, thread_ascolto, &par[i]);
        } else {
            // Processo figlio
            // chiusura del lato di ascolto della pipe
            close(fd[0]);
            // Ridirezione dello standard output verso il lato in scrittura della pipe
            dup2(fd[1], 1);
            // Avvio del processo giocatore
            execl("./passo1", "passo1", NULL);//null per terminare la lista, il primo è argv[0], il nome del programma
            // L'invocazione della exec*() causa l'abbandono di questo codice.
        }
    }
    // Processo genitore: alla fine della generazione dei 10 giocatori, inizia a stampare periodicamente le statistiche
    stampa_statistiche(&mutex, pids, punteggi_parziali, &punteggio_totale);
}
