#include <stdlib.h>
#include <stdio.h>
#include "strategy/strategy.h"

int main(void) {
    struct Strategy strat;
    Strategy_new(&strat);
    return Strategy_start(&strat);
}
