#include <raylib.h>
// #include <stdbool.h>

typedef struct InputConfig InputConfig;
struct InputConfig
{
    Vector3 playerPosition;
    int showConsole;
    float char_speed;
};

typedef struct InputOut InputOut;
struct InputOut
{
    Vector3 playerPosition;
    int showConsole;
    int animIndex;
};

typedef struct InputEvent_State InputEvent_State;
struct InputEvent_State
{
    int KEY_F1_press;
};

InputEvent_State InitInputEvent();

InputOut ExecuteInputEvent(InputEvent_State state, InputConfig cfg);