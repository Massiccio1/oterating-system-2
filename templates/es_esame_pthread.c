#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <pthread.h>

/*
* autogenerated by chatgpt
*/

// Function to be executed by the thread

#define N_THREAD 4


typedef struct th_par{
    int id;
    int data;
    char* stringa;
    pthread_mutex_t* mutex;
}th_par;

void* threadFunction(void* arg)
{
    th_par* par = (th_par*)arg;
    // Do some work in the thread
    printf("\nThread %d avviato", par->id);
    while(1){
        printf("\nThread %d bloccato", par->id);
        fflush(stdout);
        while(!par->data){
            //aspetto che diventi true
        }
        par->data=0;
        printf("\nThread %d sbloccato", par->id);
        fflush(stdout);
        sleep(5);
    }
    pthread_exit((void*)arg);
    return (void*)arg;
}

int main(){

    mkfifo("main_fifo",777);

    pthread_t thread[N_THREAD];
    int arg_th[N_THREAD];
    th_par par[N_THREAD];

    for(int i=0; i< N_THREAD; i++){
        arg_th[i]=i;
        par[i].id=i;
        par[i].data=0;
        pthread_create(&thread[i], NULL, threadFunction, &par[i] );
    }

    printf("avviato tutto");

    int fd = open("main_fifo", O_RDONLY);
    char arr1[80], arr2[80];
    
    while(1){
        //printf("sto leggendo...");
        int ret = read(fd, arr1, sizeof(arr1));
        //printf("letto da fifo: %d caratteri: %s",ret,arr1);
        if (ret >0 ){
            int id= atoi(arr1);
            printf("\nletto da fifo: %d \n",id);
            par[id].data = 1;
            if(id==-1)
                break;
        }
    }
    // Wait for the thread to finish
    /*
    for(int i=0; i< N_THREAD; i++){
        arg_th[i]=i;
        pthread_join(thread[i], NULL);
    }
    */
    unlink("main_fifo");

    printf("fine");
    return 0;
}
