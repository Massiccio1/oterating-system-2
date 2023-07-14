//#include

//#include "doppio.h"
#include <ctype.h>
#include <errno.h>     // errno
#include <fcntl.h>     // O_...
#include <pthread.h>
#include <signal.h>
#include <stdio.h>     // printf, fprintf
#include <stdlib.h>    // exit
#include <string.h>    // strcpy
#include <sys/msg.h>   // queue
#include <sys/stat.h>
#include <sys/types.h> // fqueue
#include <unistd.h>

#include <headrer.h>

#define N_THREAD 4

//typedef struct
typedef struct th_par{
    int id;
    int data;
    char* stringa;
    pthread_mutex_t* mutex;
}th_par;

//thread FUNC

void* threadFunction(void* arg){
    th_par* par = (th_par*)arg;//converto punt aw void in punt a th_par
    pthread_mutex_lock(par->mutex);//con mutex puntatore, altrimenti &
    pthread_mutex_unlock(par->mutex);//con mutex puntatore, altrimenti &
    pthread_exit((void*)arg);
    return NULL;
}
//segnali
int mode;
void sig_func(int segnale) {
    if (segnale == SIGUSR1)
        mode = 1; // SIGUSR1 inibisce la mode o->x
    else if (segnale == SIGUSR2)
        mode = 2; // SIGUSR2 la ripristina
}


int main(int argc, char *argv[]){
    // segnali
    signal(SIGUSR1, sig_func);
    signal(SIGUSR2, sig_func);
    // Ciclo infinito, pause() si risveglia ad ogni segnale ricevuto 
    while (1) pause();

    int rand = 1321321;
    th_par par[N_THREAD];
    //mutex
    pthread_mutex_t mut;
    pthread_mutex_init(&mut, NULL);

    //sctruct
    th_par my_par;
    my_par.mutex=&mut;

    //open read write, dup2
    int fd[2];
    pipe(fd);
    dup2(fd[1], 2);//out in err
    close(fd[0]);//chiudo ascolto
    int random_fd = open("/dev/random", O_RDONLY);
    int fd_null = open("/dev/null", O_WRONLY);
    char buffer[100];
    int l = read(random_fd, buffer, sizeof(buffer));
    // assumiamo che la stringa letta termini con un a capo, lo sostituiamo con un terminatore.
    buffer[l] = 0;
    write(fd_null, &rand, sizeof(rand));


    //fifo
    char * myfifo = "myfifo";
    mkfifo(myfifo, 0x111111);
    int fd_fifo = open(myfifo, O_WRONLY);
    
    //thread CALL
    pthread_t tid;
    pthread_t t_vect[N_THREAD];
    pthread_create(&tid, NULL, threadFunction, &my_par);//null per il return
    for (int i = 0; i < N_THREAD; i++) {
		void *valore;
		pthread_join(t_vect[i], &valore);//valore passato dopo malloc
		printf("Thread %d concluso.\n", i);
	}

    //exec
    execl("./passo1", "passo1", NULL);

    //fork
    int pid_figlio = fork();
     if (pid_figlio)//guarda se il pid è 0
        printf("\ngenitore");
    else//il figlio riceve return 0 dalla fork
        printf("\nfiglio");

    //puts gets sprintf
    puts("Ciao, mondo!");
	fflush(stdout);
    char buffer_1;
    // Leggi un carattere dallo standard input
    buffer_1 = getchar();
    putchar(buffer_1);
    char buffer_2[100];
    // inserire il valore come stringa nel buffer, terminato con un a capo
    sprintf(buffer, "%d\n", rand);

    //enum
    enum stato_lampadina {
        SPENTA,
        ACCESA
    };

    //ternario
    enum stato_lampadina nuovo_stato = rand % 2 == 0 ? ACCESA : SPENTA;
    printf("Lampadina %s.\n", nuovo_stato == ACCESA ? "accesa" : "spenta");

    //malloc
    int numero_interruttori = atoi(argv[1]);
	// Alloca spazio per il vettore dei descrittori in lettura delle pipe, da passare alla centrallina
	int *descrittori_lettura = (int*)malloc(numero_interruttori * sizeof(int));

    return 0;
}
