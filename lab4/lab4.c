#include <stdio.h>
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/neutrino.h>

#define SEC_NSEC 1000000000LL

void *long_thread(void *notused) {
    int time = 20;
    printf("Thread will be executed for %d seconds\n", time);
    sleep(time);
}

int main(void){
    uint64_t timeout;
    struct sigevent event;
    int rval;
    pthread_t thread_id;

    printf("Timer\n");

    event.sigev_notify = SIGEV_UNBLOCK; 
    pthread_create(&thread_id, NULL, long_thread, NULL);

    timeout = 10LL * SEC_NSEC;
    TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_JOIN, &event, &timeout, NULL);
    rval = pthread_join(thread_id, NULL);
    if (rval == ETIMEDOUT) printf("Thread %d executes for more than 10 seconds\n", thread_id);
    TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_JOIN, &event, &timeout, NULL);
    sleep(5);
    rval = pthread_join(thread_id, NULL);
    if (rval == ETIMEDOUT) printf("Thread %d executes for more than 25 seconds\n", thread_id);
    else printf("Thread %d is finished\n", thread_id);

    return (1);
}
