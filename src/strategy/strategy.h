#ifndef Strategy_H
#define Strategy_H

#include "feed/quote.h"

struct Strategy{};

void Strategy_new(struct Strategy *);

int Strategy_start(struct Strategy *);

void Strategy_estimateLiquidity(struct Strategy *, struct Quote quote, double a);

void Strategy_estimateVolatility(struct Strategy *, struct Quote quote, double a);

#endif