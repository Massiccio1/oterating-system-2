// pid_t
#include <sys/types.h>
// pthread_mutex_t
#include <pthread.h>

// Numero di processi da generare
#define N_PROCESSI 10

// Parametri da passare a ciascun thread
struct parametri_thread {
    // process ID del figlio del quale il thread si mette in ascolto
    pid_t pid_figlio;
    // puntatore all'accumulatore del punteggio del processo figlio
    float *punteggio_parziale;
    // puntatore all'accumulatore dei punteggi
    float *punteggio_totale;
    // descrittore del lato in ascolto della pipe verso il processo figlio
    int fd_ascolto;
    // mutex per l'accesso alla variabile condivisa *punteggio_totale
    pthread_mutex_t *mutex;
};

// thread di ascolto del processo figlio
void *thread_ascolto(void *p);

// funzione di stampa delle statistiche
void stampa_statistiche(pthread_mutex_t *mutex, pid_t pids[], float parziali[], float *totale);
