#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int* fork_n(int nchild, int progenitor, int* child){

    int ch=0;

    for(int i=0; i< nchild; i++){
        int ret_f = fork();
        
        if(getpid()!=progenitor){//figlio
            //printf("creato figlio: %d\n",getpid());
            ch=i+1;
            return NULL;
        }
        child[i]=ret_f;
    }
    return child;
}

int main(int argc, char**argv){

    int nchild = atoi(argv[1]);
    int progenitor=getpid();

    printf("inizio: %d\n\n",getpid());

    int child[nchild];
    fork_n(nchild,progenitor, child);

    //
    //printf("\nil mio pid: %d, sono tipo: %d\n",getpid(),ch);
    //fflush(stdout);

    if(getpid()!=progenitor){//figlio
        printf("pid: %d, figlio\n",getpid());
    }else{//padre
        sleep(1);
        printf("pid: %d, padre\n",getpid());
        printf("\n-------------\n");
        //printf("\nin padre %d\n con lista di figli: ",getpid());
        for(int i=0; i< nchild;i++){
            printf("%d, ",child[i]);
        }
        printf("\nfine main padre\n");
    }
    
    return 0;
}