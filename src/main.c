#include <stdio.h>
#include "sig/clock.h"

void handler(int sig) {
    printf("Hello, signal %d\n", sig);
}

int main(void) {
    struct SigClock s;
    SigClock_new(&s, 1, 4, handler);
    return SigClock_start(&s);
}
