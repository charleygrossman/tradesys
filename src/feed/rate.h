#ifndef Rate_H
#define Rate_H

#include <stdint.h>

struct Rate {
    char symbol[5];
    int64_t price;
    int64_t quantity;
};

struct Rate genRate();

#endif