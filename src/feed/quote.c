#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define QUOTE_FILE_HEADER u8"symbol,min_price,max_price,min_quantity,max_quantity"
#define QUOTE_SYMBOL_SIZE 4

// TODO: Support for decimal values.
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

struct Quote randQuote(struct QuoteConfig cfg) {
    struct Quote q = {
        .askQuantity = rand() % (cfg.maxQuantity + 1 - cfg.minQuantity) + cfg.minQuantity,
        .bidPrice = rand() % (cfg.maxPrice + 1 - cfg.minPrice) + cfg.minPrice,
        .bidQuantity = rand() % (cfg.maxQuantity + 1 - cfg.minQuantity) + cfg.minQuantity,
    };
    q.askPrice = rand() % (cfg.maxPrice + 1 - q.bidPrice) + q.bidPrice + 1;
    for (int i = 0; i < QUOTE_SYMBOL_SIZE; i++) {
        q.symbol[i] = cfg.symbol[i];
    }
    return q;
}

int parseQuoteConfigs(const char *filepath, struct QuoteConfig **quoteConfigs, size_t *quoteConfigsSize) {
    FILE *f;
    if ((f = fopen(filepath, "r")) == NULL) {
        return EXIT_FAILURE;
    }

    char header[strlen(QUOTE_FILE_HEADER)];
    if (fscanf(f, "%s", header) == EOF || header == NULL || strcmp(header, QUOTE_FILE_HEADER) != 0) {
        goto FAIL_CLOSE;
    }

    size_t size = 0;
    struct QuoteConfig q;
    do {
        int n = fscanf(f, "%3s,%lu,%lu,%lu,%lu", q.symbol, &q.minPrice, &q.maxPrice, &q.minQuantity, &q.maxQuantity);
        if (n == 5) {
            size++;
        } else if (n == EOF) {
            break;
        } else {
            goto FAIL_CLOSE;
        }
    } while(true);
    if (size == 0) {
        if (fclose(f) != 0) {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
    if (fseek(f, sizeof(header), SEEK_SET) != 0) {
        goto FAIL_CLOSE;
    }

    struct QuoteConfig *v = NULL;
    v = (struct QuoteConfig *)reallocarray(*quoteConfigs, size, sizeof(struct QuoteConfig));
    if (v == NULL) {
        goto FAIL_CLOSE_FREE;
    }
    *quoteConfigs = v;
    for (size_t i = 0; i < size; i++) {
        struct QuoteConfig q;
        if (fscanf(f, "%3s,%lu,%lu,%lu,%lu", q.symbol, &q.minPrice, &q.maxPrice, &q.minQuantity, &q.maxQuantity) != 5) {
            goto FAIL_CLOSE_FREE;
        }
        (*quoteConfigs)[i] = q;
    }
    *quoteConfigsSize = size;
    return EXIT_SUCCESS;

    FAIL_CLOSE:
        fclose(f);
        return EXIT_FAILURE;
    FAIL_CLOSE_FREE:
        free(*quoteConfigs);
        *quoteConfigs = NULL;
        fclose(f);
        return EXIT_FAILURE;
}

void marshalString(unsigned char *data, char *v, size_t size) {
    for (size_t i = 0; i < size; i++) {
        data[i] = v[i];
    }
}

void unmarshalString(char *v, unsigned char *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        v[i] = data[i];
    }
}

void marshalUint64(unsigned char *data, uint64_t v) {
    *data++ = v>>56;
    *data++ = v>>48;
    *data++ = v>>40;
    *data++ = v>>32;
    *data++ = v>>24;
    *data++ = v>>16;
    *data++ = v>>8;
    *data++ = v;
}

uint64_t unmarshalUint64(unsigned char *data) {
    return ((uint64_t) data[0]<<56) |
        ((uint64_t) data[1]<<48) |
        ((uint64_t) data[2]<<40) |
        ((uint64_t) data[3]<<32) |
        ((uint64_t) data[4]<<24) |
        ((uint64_t) data[5]<<16) |
        ((uint64_t) data[6]<<8) |
        data[7];
}

void marshalQuote(unsigned char *data, struct Quote *quote) {
    marshalString(data, quote->symbol, QUOTE_SYMBOL_SIZE);
    data += QUOTE_SYMBOL_SIZE;
    marshalUint64(data, quote->askPrice);
    data += 8;
    marshalUint64(data, quote->askQuantity);
    data += 8;
    marshalUint64(data, quote->bidPrice);
    data += 8;
    marshalUint64(data, quote->bidQuantity);
}

void unmarshalQuote(struct Quote *quote, unsigned char *data) {
    unmarshalString(quote->symbol, data, QUOTE_SYMBOL_SIZE);
    data += QUOTE_SYMBOL_SIZE;
    quote->askPrice = unmarshalUint64(data);
    data += 8;
    quote->askQuantity = unmarshalUint64(data);
    data += 8;
    quote->bidPrice = unmarshalUint64(data);
    data += 8;
    quote->bidQuantity = unmarshalUint64(data);
}