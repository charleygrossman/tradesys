#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include "feed/quote.h"
#include "feed/server.h"
#include "sig/clock.h"

volatile sig_atomic_t feedFlag;

void *feedStart(void *);
void feedHandler(int);

struct Feed {
    struct QuoteConfig *quoteConfigs;
    size_t quoteConfigsSize;
};

int Feed_new(struct Feed *feed, const char *quoteFilepath) {
    memset(feed, 0, sizeof(struct Feed));
    feed->quoteConfigs = NULL;
    return parseQuoteConfigs(quoteFilepath, &feed->quoteConfigs, &feed->quoteConfigsSize);
}

int Feed_start(struct Feed *feed) {
    struct Server server;
    if (Server_start(&server, "50000") != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
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
            for (size_t i = 0; i < feed->quoteConfigsSize; i++) {
                if (Server_sendQuote(&server, randQuote(feed->quoteConfigs[i])) != EXIT_SUCCESS) {
                    return EXIT_FAILURE;
                }
            }
            feedFlag = false;
        }
    }
    return EXIT_SUCCESS;
}

void *feedStart(__attribute__((unused)) void *data) {
    struct SigClock s;
    SigClock_new(&s, 1, 1, feedHandler);
    return (void *)(intptr_t)SigClock_start(&s);
}

void feedHandler(__attribute__((unused)) int sig) {
    feedFlag = true;
}