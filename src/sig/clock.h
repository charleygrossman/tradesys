#ifndef SigClock_H
#define SigClock_H

#include <stdlib.h>
#include <signal.h>

// SigClock represents a monotonic clock
// with a tick frequency and limit that
// invokes the provided signal handler
// every tick.
struct SigClock {
    // frequency represents the time interval 
    // between each clock tick, in seconds.
    u_int64_t frequency;
    // limit represents the number of ticks
    // that the clock should take.
    u_int64_t limit;
    // sa represents the underlying signal handler sigaction.
    struct sigaction sa;
    // timerID represents the ID of the underlying timer.
    timer_t timerID;
};

void SigClock_new(struct SigClock *, u_int64_t freq, u_int64_t limit, void handler(int sig));
int SigClock_start(struct SigClock *);

#endif