#ifndef Quote_H
#define Quote_H

#include <stdint.h>
#include <stddef.h>

#define QUOTE_SYMBOL_SIZE 4

struct Quote {
    char symbol[QUOTE_SYMBOL_SIZE];
    int64_t askPrice, askQuantity;
    int64_t bidPrice, bidQuantity;
};

struct QuoteConfig {
    char symbol[QUOTE_SYMBOL_SIZE];
    int64_t minPrice, maxPrice;
    int64_t minQuantity, maxQuantity; 
};

struct Quote randQuote(struct QuoteConfig cfg);

int parseQuoteConfigs(const char *filepath, struct QuoteConfig **quoteConfigs, size_t *quoteConfigsSize);

#endif