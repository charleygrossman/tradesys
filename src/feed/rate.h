#ifndef Rate_H
#define Rate_H

#include <stdint.h>

#define RATE_SYMBOL_SIZE 4

struct Rate {
    char symbol[RATE_SYMBOL_SIZE];
    int64_t price;
    int64_t quantity;
};

struct RateConfig {
    char symbol[RATE_SYMBOL_SIZE];
    int64_t minPrice, maxPrice;
    int64_t minQuantity, maxQuantity; 
};

struct Rate randRate(struct RateConfig cfg);

int parseRateConfigs(const char *filepath, struct RateConfig **rateConfigs, size_t *rateConfigsSize);

#endif