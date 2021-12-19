#include <stdlib.h>
#include <stdint.h>

#define ALPHABET u8"ABCDEFGHIJKLMNOPQRSTUVWXYZ"

struct Rate {
    char symbol[5];
    int64_t price;
    int64_t quantity;
};

struct Rate genRate() {
    struct Rate r = {
        .price = (rand() % 10) + 1,
        .quantity = (rand() % 100) + 1,
    };
    for (int i = 0; i < 4; i++) {
        r.symbol[i] = ALPHABET[rand() % 26];         
    }
    return r;
}
