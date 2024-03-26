
static int TIME_1_SECOND = 1 * 1000;

typedef struct TickState
{
    int rateInHz;
    int current;
    int lastUpdate;
} TickState;

TickState InitTick(int tickRateInHz);

void StartTick(TickState *state);

int IsTickUpdate(TickState *state);

int UpdateTick(TickState *state);
