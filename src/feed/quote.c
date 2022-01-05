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
    int64_t askPrice, askQuantity;
    int64_t bidPrice, bidQuantity;
};

struct QuoteConfig {
    char symbol[QUOTE_SYMBOL_SIZE];
    int64_t minPrice, maxPrice;
    int64_t minQuantity, maxQuantity; 
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
        int n = fscanf(f, "%3s,%ld,%ld,%ld,%ld", q.symbol, &q.minPrice, &q.maxPrice, &q.minQuantity, &q.maxQuantity);
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
        if (fscanf(f, "%3s,%ld,%ld,%ld,%ld", q.symbol, &q.minPrice, &q.maxPrice, &q.minQuantity, &q.maxQuantity) != 5) {
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