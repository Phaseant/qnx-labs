#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <sys/neutrino.h>

typedef struct thread_args {
    int id;
    int n;
    const char* msg;
} thread_args;

void *thread_func(void *args) {    
    thread_args* cur_args = (thread_args*)args;
    printf("Thread %d: started execution. n = %d\n", cur_args->id, cur_args->n);
    printf("Thread %d: %s\n", cur_args->id, cur_args->msg);

    sleep(cur_args->n);
    printf("Thread %d: finished execution\n", cur_args->id);
    pthread_exit(NULL);
}

void create_thread(pthread_t* tid, int policy, int prio, void*(func)(void*), void* args)
{
    pthread_attr_t attr;
    struct sched_param param;

    param.sched_priority = prio;
    pthread_attr_init(&attr);
    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setschedpolicy(&attr, policy);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(tid, &attr, func, args);

    pthread_attr_destroy(&attr);
}

int main() {
    pthread_t thread_id1, thread_id2;
    int priority_1 = 18;
    int priority_2 = 4;

    thread_args args1 = {1, 5, "Message from the first thread\n"};
    thread_args args2 = {2, 10, "Message from the second thread\n"};

    create_thread(&thread_id1, SCHED_FIFO, priority_1, thread_func, (void*)&args1);
    create_thread(&thread_id2, SCHED_RR, priority_2, thread_func, (void*)&args2);

    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);

    return 0;
}
