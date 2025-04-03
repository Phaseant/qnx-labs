#include <stdio.h>
#include <time.h>
#include <sync.h>
#include <pthread.h>
#include <sys/neutrino.h>

barrier_t barrier;

void *thread1(void * not_used)
{
    char buf[27];
    time_t now;

    printf("Start waiting for barrier in 1 thread\n");
    sleep(1);
    barrier_wait(&barrier);
    printf("Finish waiting for barrier in 1 thread\n");
}

void *thread2(void * not_used)
{
    char buf[27];
    time_t now;

    printf("Start waiting for barrier in 2 thread\n");
    sleep(2);
    barrier_wait(&barrier);
    printf("Finish waiting for barrier in 2 thread\n");
}


void main()
{
    char buf[27];
    time_t now;
    int NUM_THREADS = 2;
    pthread_t t1;
    pthread_t t2;

    //барьер, атрибуты, число потоков + 1, т.к. мейн поток
    barrier_init(&barrier, NULL, NUM_THREADS + 1); 
    printf("Program start\n");
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    printf("Start waiting for barrier in main\n");
    barrier_wait(&barrier);
    printf("Finish waiting for barrier in main\n");

    pthread_join(t1, NULL);
}