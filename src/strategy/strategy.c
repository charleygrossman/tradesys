#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include "feed/quote.h"
#include "strategy/client.h"
#include "sig/clock.h"

struct Strategy {
    double liquidity;
    double volatility;
};

volatile sig_atomic_t stratFlag;

void *strategyStart(void *);
void strategyHandler(int);

void Strategy_new(struct Strategy *strat) {
    memset(strat, 0, sizeof(struct Strategy));
}

// TODO: Process quotes and calculate estimates.
int Strategy_start(struct Strategy __attribute__((unused)) *strat) {
    struct Client client;
    if (Client_start(&client, "50000") != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    pthread_t stratStartID;
    void *stratStartRetval;
    if (pthread_create(&stratStartID, NULL, strategyStart, NULL) != 0) {
        return EXIT_FAILURE;
    }
    if (pthread_join(stratStartID, &stratStartRetval) != 0 || (intptr_t)stratStartRetval != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    srand(time(NULL));
    while (true) {
        if (stratFlag) {
            if (Client_recvQuote(&client) != EXIT_SUCCESS) {
                return EXIT_FAILURE;
            }
            stratFlag = false;
        }
    }
    return EXIT_SUCCESS;
}

void *strategyStart(__attribute__((unused)) void *data) {
    struct SigClock s;
    SigClock_new(&s, 1, 1, strategyHandler);
    return (void *)(intptr_t)SigClock_start(&s);
}

void strategyHandler(__attribute__((unused)) int sig) {
    stratFlag = true;
}

void Strategy_estimateLiquidity(struct Strategy *strat, struct Quote quote, double a) {
    strat->liquidity = (1-a)*(quote.askQuantity+quote.bidQuantity) + a*strat->liquidity;   
}

void Strategy_estimateVolatility(struct Strategy *strat, struct Quote quote, double a) {
    strat->volatility = (1-a)*pow(quote.askQuantity+quote.bidQuantity-strat->liquidity, 2) + a*strat->volatility;
}
