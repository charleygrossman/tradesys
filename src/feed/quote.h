#ifndef Quote_H
#define Quote_H

#include <stdint.h>
#include <stddef.h>

#define QUOTE_SYMBOL_SIZE 4

struct Quote {
    char symbol[QUOTE_SYMBOL_SIZE];
    uint64_t askPrice, askQuantity;
    uint64_t bidPrice, bidQuantity;
};

struct QuoteConfig {
    char symbol[QUOTE_SYMBOL_SIZE];
    uint64_t minPrice, maxPrice;
    uint64_t minQuantity, maxQuantity; 
};

struct Quote randQuote(struct QuoteConfig cfg);

int parseQuoteConfigs(const char *filepath, struct QuoteConfig **quoteConfigs, size_t *quoteConfigsSize);

void marshalQuote(unsigned char *data, struct Quote *quote);

void unmarshalQuote(struct Quote *quote, unsigned char *data);

#endif