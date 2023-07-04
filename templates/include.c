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


int main(int argc, char **argv) {
    int i=2;
    printf("finito con exit(%d)\n",i);
    return i;
}