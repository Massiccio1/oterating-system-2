#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int func(int a, int b){
    int c = a+b;
    return c*3;
}

int main(){
    int fd[2];

    int isfatcher = fork();
    if(!isfatcher){//figlio

        int pid = getpid();
        printf("%d\n",pid);
        sleep(1);
        
    }else{
        while(1){
            printf("Ciao, mondo\n");
            sleep(1);
        }
    }


    return 0;
}