#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int stato = 0;

typedef struct th_par{//struct per passare i parametri *void al thread
    int fd;
    int ch;
    char* stringa;
    pthread_mutex_t* mutex;
}th_par;

void* ascolto(void * arg){
    
    int stato=0;
    th_par* t = (th_par*)arg;//parsing, così posso accedere all struct
    int fd = t->fd;
    int ch = t->ch;
    char msg[2];
    printf("\nsto ascoltanto %d su fd: %d ",ch,fd);
    while(1){
        int n_read = read(fd,&msg,2);
        if(n_read>0){
            printf("\ninterruttore %d ha scritto %d con %d caratteri\n",ch,atoi(msg),n_read);
            fflush(stdout);
            sleep(1);
        }
    }
    printf("\nfine ascolto");
    return arg;
}

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
            break;
        }
        child[i]=ret_f;
    }
    sleep(1);
    //
    //printf("\nil mio pid: %d, sono tipo: %d\n",getpid(),ch);
    //fflush(stdout);

    if(ch){//figlio
        close(pipes[ch-1][0]);//chudo lettura
        interruttore(ch, pipes[ch-1][1] );
    }else{//padre
        sleep(1);
        pthread_t thread[nchild];
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