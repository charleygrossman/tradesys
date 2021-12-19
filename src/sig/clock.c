#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>

struct SigClock {
    uint64_t frequency;
    uint64_t limit;
    struct sigaction sa;
    timer_t timerID;
};

void SigClock_new(struct SigClock *sigClock, uint64_t freq, uint64_t limit, void handler(int sig)) {
    memset(sigClock, 0, sizeof(struct SigClock));
    sigClock->frequency = freq;
    sigClock->limit = limit;
    sigClock->sa.sa_handler = handler;
}

int SigClock_start(struct SigClock *sigClock) {
    if (sigemptyset(&sigClock->sa.sa_mask) != 0) {
        return EXIT_FAILURE;
    }
    if (sigaction(SIGRTMIN, &sigClock->sa, NULL) != 0) {
        return EXIT_FAILURE;
    }

    struct sigevent sigEvt;
    memset(&sigEvt, 0, sizeof(struct sigevent));
    sigEvt.sigev_notify = SIGEV_SIGNAL;
    sigEvt.sigev_signo = SIGRTMIN;
    sigEvt.sigev_value.sival_ptr = &sigClock->timerID;
    if (timer_create(CLOCK_REALTIME, &sigEvt, &sigClock->timerID) != 0) {
        return EXIT_FAILURE;
    }

    struct itimerspec timerSpec;
    memset(&timerSpec, 0, sizeof(struct itimerspec));
    timerSpec.it_interval.tv_sec = sigClock->frequency;
    timerSpec.it_value.tv_sec = sigClock->frequency;
    if (timer_settime(sigClock->timerID, 0, &timerSpec, NULL) != 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


