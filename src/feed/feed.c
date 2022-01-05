#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include "feed/quote.h"
#include "sig/clock.h"

volatile sig_atomic_t feedFlag;

void *feedStart(void *);
void handler(int);

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
                struct Quote q = randQuote(feed->quoteConfigs[i]);
                printf("symbol=%s ask price=%ld ask quantity=%ld bid price=%ld bid quantity=%ld\n",
                    q.symbol, q.askPrice, q.askQuantity, q.bidPrice, q.bidQuantity
                );
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