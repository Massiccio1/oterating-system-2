#include <stdio.h>
#include <cstdlib>
#include <pthread.h>

/*
* autogenerated by chatgpt
*/

// Function to be executed by the thread


typedef struct th_par{
    int data;
    char* stringa;
    pthread_mutex_t* mutex;
}th_par;

void* threadFunction(void* arg)
{
    // Do some work in the thread
    printf("\nThread Function Executing with args: %s\n", (char*)arg);
    int *p;
    p = (int *) malloc (1* sizeof *p);
    *p=55;
    printf("test return inside thread: %d\n",*p);
    printf("test return inside thread: %d\n", *(int*)(void*)p);
    pthread_exit((void*)p);
    return (void*)p;
}

int main()
{
    pthread_t thread;
    int thread_create_result;
    void* ret;
    // Create the thread
    char s[100]= "123testing multiple arguments";
    thread_create_result = pthread_create(&thread, NULL, threadFunction, &s);
    if (thread_create_result)
    {
        printf("Error: Unable to create thread, %d ", thread_create_result);
        return 1;
    }

    // Wait for the thread to finish
    int t = pthread_join(thread, &ret);

    //printf("Thread joined with return: %d ", *(int*)ret);
    printf("Thread %d joined with return: %d\n", t, *(int*)ret);
    free(ret);
    return 0;
}
