#include <stdlib.h>
#include <stdio.h>
#include "doppio.h"

int func(int a, int b){
    int c = a+b;
    return c*3;
}

int main(){
    int i=12;
    //printf("\nexiting with %d",i);
    int a = doppio(12);
    return func(a,0);
}