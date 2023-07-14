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

int fd[2];
int r = 1;


void sig_func(int segnale) {
    char buffer[100];
    // int l = read(fd[0], buffer, sizeof(buffer));
    // // assumiamo che la stringa letta termini con un a capo, lo sostituiamo con un terminatore.
    // buffer[l] = 0;
    printf("in callback\n");    
    // sprintf(buffer, "%d\n",r);
    // r++;
    // write(fd_fifo, &buffer, sizeof(buffer));
}

int main(){

    signal(SIGUSR1, sig_func);
    
    pipe(fd);
    
    // dup2(fd[0],0);
    // dup2(fd[1],1);

    printf("pid: %d",getpid());
    fflush(stdout);

    char * myfifo = "fifo";
    int fd_fifo = open(myfifo, O_RDONLY);

    while(1){

        char buffer[100];
        int l = read(fd_fifo, buffer, sizeof(buffer));
        buffer[l] = 0;
        // assumiamo che la stringa letta termini con un a capo, lo sostituiamo con un terminatore.
        printf("letto: %s",buffer);
    }
}