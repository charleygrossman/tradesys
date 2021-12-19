#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include "feed/rate.h"
#include "sig/clock.h"

volatile sig_atomic_t feedFlag;

void *feedStart(void *);
void handler(int);

int feed(void) {
    pthread_t feedStartID;
    void *feedStartRetval;
    if (pthread_create(&feedStartID, NULL, feedStart, NULL) != 0) {
        return EXIT_FAILURE;
    }
    if (pthread_join(feedStartID, &feedStartRetval) != 0 || (intptr_t)feedStartRetval != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    srand(time(NULL));
    while (true) {
        if (feedFlag) {
            struct Rate r = genRate();
            printf("symbol=%s price=%ld quantity=%ld\n", r.symbol, r.price, r.quantity);
            feedFlag = false;
        }
    }
    return EXIT_SUCCESS;
}

void *feedStart(__attribute__((unused)) void *data) {
    struct SigClock s;
    SigClock_new(&s, 1, 1, handler);
    return (void *)(intptr_t)SigClock_start(&s);
}

void handler(__attribute__((unused)) int sig) {
    feedFlag = true;
}
