#include <math.h>
#include "feed/quote.h"

struct Strategy {
    double liquidity;
    double volatility;
};

void Strategy_estimateLiquidity(struct Strategy *strat, struct Quote quote, double a) {
    strat->liquidity = (1-a)*(quote.askQuantity+quote.bidQuantity) + a*strat->liquidity;   
}

void Strategy_estimateVolatility(struct Strategy *strat, struct Quote quote, double a) {
    strat->volatility = (1-a)*pow(quote.askQuantity+quote.bidQuantity-strat->liquidity, 2) + a*strat->volatility;
}
