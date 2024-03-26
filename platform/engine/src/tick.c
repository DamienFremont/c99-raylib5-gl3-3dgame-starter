#include "tick.h"

#include <time.h>

static clock_t Tick_StartClock = {0};
static clock_t Tick_CurrentClock = {0};

TickState InitTick(int tickRateInHz)
{
    return (TickState){
        tickRateInHz,
        1,
        0};
}

void StartTick(TickState *state)
{
    Tick_StartClock = clock();
    state->current = 0;
    state->lastUpdate = 0;
}

int IsTickUpdate(TickState *state)
{
    Tick_CurrentClock = clock();
    // compute
    clock_t diff = Tick_CurrentClock - Tick_StartClock;
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
