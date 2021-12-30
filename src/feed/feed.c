#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include "feed/rate.h"
#include "sig/clock.h"

volatile sig_atomic_t feedFlag;

void *feedStart(void *);
void handler(int);

struct Feed {
    struct RateConfig *rateConfigs;
    size_t rateConfigsSize;
};

int Feed_new(struct Feed *feed, const char *rateFilepath) {
    memset(feed, 0, sizeof(struct Feed));
    feed->rateConfigs = NULL;
    return parseRateConfigs(rateFilepath, &feed->rateConfigs, &feed->rateConfigsSize);
}

int Feed_start(struct Feed *feed) {
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
            for (size_t i = 0; i < feed->rateConfigsSize; i++) {
                struct Rate r = randRate(feed->rateConfigs[i]);
                printf("symbol=%s price=%ld quantity=%ld\n", r.symbol, r.price, r.quantity);
            }
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