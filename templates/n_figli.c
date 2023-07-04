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

int main(int argc, char**argv){

    int nchild = atoi(argv[1]);

    int pipes[nchild][2];
    int child[nchild];
    int progenitor=getpid();

    printf("inizio: %d\n\n",getpid());

    int ch=0;

    for(int i=0; i< nchild;i++){
        pipe(pipes[i]);
    }

    for(int i=0; i< nchild; i++){
        int ret_f = fork();
        
        if(getpid()!=progenitor){//figlio
            //printf("creato figlio: %d\n",getpid());
            ch=i+1;
            break;//se sei figlio, esci e non crearne altri
        }
        child[i]=ret_f;
    }
    sleep(1);
    //
    //printf("\nil mio pid: %d, sono tipo: %d\n",getpid(),ch);
    //fflush(stdout);

    if(ch){//figlio
        close(pipes[ch-1][0]);//chudo lettura
        //fai qualcosa
    }else{//padre
        sleep(1);
        pthread_t thread[nchild];// n thread, 1 per figlio
        int arg_th[nchild];
        th_par par[nchild];
        printf("\n-------------");
        //printf("\nin padre %d\n con lista di figli: ",getpid());
        for(int i=0; i< nchild;i++){
            //printf("%d\n",child[i]);
            //fflush(stdout);
            close(pipes[i][1]);//chiudo scrittura

            par[i].ch=i;
            par[i].fd=pipes[i][0];
            pthread_create(&thread[i], NULL, ascolto, &par[i] );
        }
        /*
        for(int i=0; i< nchild;i++){
            kill(child[i],SIGUSR1);
        }
        sleep(1);
        for(int i=0; i< nchild;i++){
            kill(child[i],SIGUSR2);
        }
        */
        while(1);
    }
    printf("fine main");
    return 0;
}