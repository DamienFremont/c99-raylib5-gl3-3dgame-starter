#include "tick.h"

#include <time.h>

TickState InitTick(int tickRateInHz)
{
    return (TickState){
        tickRateInHz,
        1,
        0};
}

void StartTick(TickState *state)
{
    state->startClock = clock();
    state->current = 0;
    state->lastUpdate = 0;
}

int IsTickUpdate(TickState *state)
{
    state->currentClock = clock();
    // compute
    clock_t diff = state->currentClock - state->startClock;
    int diffInMs = diff * 1000 / CLOCKS_PER_SEC;
    // restart
    if (diffInMs > TIME_1_SECOND)
    {
        StartTick(state);
        return 0;
    }
    // current
    int tickInMs = (1000 / state->rateInHz);
    state->current = (diffInMs / tickInMs);
    // check
    return (state->current > state->lastUpdate);
}

int UpdateTick(TickState *state)
{
    state->lastUpdate = state->current;
}
