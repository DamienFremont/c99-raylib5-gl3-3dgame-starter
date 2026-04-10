#pragma once
#include <time.h>

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

static int TIME_1_SECOND = 1 * 1000;

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
