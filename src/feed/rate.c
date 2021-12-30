#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define RATE_FILE_HEADER u8"symbol,min_price,max_price,min_quantity,max_quantity"
#define RATE_SYMBOL_SIZE 4

// TODO: Support for decimal values.
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

struct Rate randRate(struct RateConfig cfg) {
    struct Rate r = {
        .price = (rand() % cfg.maxPrice + 1 - cfg.minPrice) + cfg.minPrice,
        .quantity = (rand() % cfg.maxQuantity + 1 - cfg.minQuantity) + cfg.minQuantity,
    };
    for (int i = 0; i < RATE_SYMBOL_SIZE; i++) {
        r.symbol[i] = cfg.symbol[i];
    }
    return r;
}

int parseRateConfigs(const char *filepath, struct RateConfig **rateConfigs, size_t *rateConfigsSize) {
    FILE *f;
    if ((f = fopen(filepath, "r")) == NULL) {
        return EXIT_FAILURE;
    }

    char header[strlen(RATE_FILE_HEADER)];
    if (fscanf(f, "%s", header) == EOF || header == NULL || strcmp(header, RATE_FILE_HEADER) != 0) {
        goto FAIL_CLOSE;
    }

    size_t size = 0;
    struct RateConfig r;
    do {
        int n = fscanf(f, "%3s,%ld,%ld,%ld,%ld", r.symbol, &r.minPrice, &r.maxPrice, &r.minQuantity, &r.maxQuantity);
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

    struct RateConfig *v = NULL;
    v = (struct RateConfig *)reallocarray(*rateConfigs, size, sizeof(struct RateConfig));
    if (v == NULL) {
        goto FAIL_CLOSE_FREE;
    }
    *rateConfigs = v;
    for (size_t i = 0; i < size; i++) {
        struct RateConfig r;
        if (fscanf(f, "%3s,%ld,%ld,%ld,%ld", r.symbol, &r.minPrice, &r.maxPrice, &r.minQuantity, &r.maxQuantity) != 5) {
            goto FAIL_CLOSE_FREE;
        }
        (*rateConfigs)[i] = r;
    }
    *rateConfigsSize = size;
    return EXIT_SUCCESS;

    FAIL_CLOSE:
        fclose(f);
        return EXIT_FAILURE;
    FAIL_CLOSE_FREE:
        free(*rateConfigs);
        *rateConfigs = NULL;
        fclose(f);
        return EXIT_FAILURE;
}