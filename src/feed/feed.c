#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>
#include "sig/clock.h"

volatile sig_atomic_t feedFlag;

void *feedStart(void *);
void handler(int);

int feed(void) {
    pthread_t threadID;
    pthread_create(&threadID, NULL, feedStart, NULL);
    pthread_join(threadID, NULL);
    while (true) {
        if (feedFlag) {
            printf("Hello, async world\n");
            feedFlag = false;
        }
    }
    return EXIT_SUCCESS;
}

void *feedStart(__attribute__((unused))void *data) {
    struct SigClock s;
    SigClock_new(&s, 1, 4, handler);
    SigClock_start(&s);
    pthread_exit(NULL);
}

void handler(__attribute__((unused)) int sig) {
    feedFlag = true;
}
