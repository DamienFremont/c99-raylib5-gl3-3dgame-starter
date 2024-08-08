#include <time.h>

#pragma once

static int TIME_1_SECOND = 1 * 1000;

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef struct TickState
{
    int rateInHz;

    int current;
    int lastUpdate;

    clock_t startClock;
    clock_t currentClock;

} TickState;

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

TickState InitTick(int tickRateInHz);
void StartTick(TickState *state);
int IsTickUpdate(TickState *state);
void UpdateTick(TickState *state);
