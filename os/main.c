#include <stdlib.h>    // exit
#include <stdio.h>     // printf, fprintf
#include <string.h>    // strcpy
#include <sys/msg.h>   // queue
#include <sys/types.h> // fqueue
#include <fcntl.h>     // O_...
#include <errno.h>     // errno
#include <signal.h> 
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int quit(int code) { // exit function
    char msg[36]="?unknown";
    if (code == 1) strcpy(msg, "args must be 2 with file path");
    if (code == 2) strcpy(msg, "syntax error");
    if (code == 3) strcpy(msg, "action unknown");
    if (code == 4) strcpy(msg, "missing value");
    if (code == 5) strcpy(msg, "unwanted value found");
    if (code == 6) strcpy(msg, "cannot create file");
    if (code == 7) strcpy(msg, "missing source queue");
    if (code>0) fprintf(stderr, "?Error #%d: %s.\n", code, msg);
    exit(code);
}

int skip1=0;
int skip2=0;

void myHandler(int sigNum){
	fflush(stdout);
    
	if(sigNum == SIGUSR1){
        printf("\nsig 1, pid: %d\n",getpid());
        if(!skip1){
            skip1++;
            kill(0,SIGUSR1);
        }else skip1--;
    }

	else if(sigNum == SIGUSR2){
        
        char line[50] = "kill -12 0";
        char num[10];
        //num = getpid() +'0';
        //strcat(line,itoa(getpid()));
        //strcat(line,getpid());
        sprintf(num,"%d",getpid());
        //printf("line: %s",line);
        //printf("num: %s",num);
        //strcat(line,num);
        printf("line: %s",line);
        //system(line);
        printf("\nskip: %d\n",skip2);
        fflush(stdout);
/*
        if(!skip2){
            skip2++;
            kill(0,SIGUSR1);
            //system(line);
        }else skip2--;
        */
       printf("\nsig 2, pid: %d\n",getpid());
        if(!skip2){
            skip2++;
            //kill(0,SIGUSR2);
            system(line);
            system("echo $$");
        }else skip2--;
    }
	fflush(stdout);
}

int main(int argc, char **argv) {
	signal(SIGUSR1,myHandler); //Ignore signal
	signal(SIGUSR2,myHandler); //Use default handler
	signal(SIGTSTP,myHandler); //Use myHandler

    printf("pid: %d\n",getpid());
	
	if(argc!=2)
        quit(1);
	int i=0;
	while(1){
		printf("\n%d",i);
		sleep(1);
	}
    return 0;
}
