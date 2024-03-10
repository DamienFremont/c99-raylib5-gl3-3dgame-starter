## Code convention

### Include syntax ?

```c
#include <stdbool.h>
```

```c
#include "stdbool.h"
```

### Memory leak monitor ?

### Bool VS Int ?

```c
int enabled = 1;
if(enabled == 1) {
    printf("Is Enabled!");
}
```

```c
#include <stdbool.h>

bool enabled = true;
if(enabled == true) {
    printf("Is Enabled!");
}
```

### Structure instanciation ?

```c
Vector3 vec = (Vector3){
    0.0f,
    1.0f,
    -1.0f};
```

```c
Vector3 vec = {0};
vec.x = 0.0f;
vec.y = 1.0f;
vec.z = -1.0f;
```

## Structure declaration ?

```c
typedef struct MyObject MyObject;
struct MyObject
{
    int number;
};
```

```c
typedef struct MyObject
{
    int number;
} MyObject;
```

## CMake vars case ?

```txt
set(mygame "../../mygame")

include_directories(
  ${mygame}/src
)
```

```txt
set(MYGAME "../../mygame")

include_directories(
  ${MYGAME}/src
)
```

## Properties visibily naming ?

```c
struct MyComp_State
{
    int counter;
};

struct MyComp_Props
{
    int increment;
};
```

```c
MyComp_State Init(MyComp_Props props);

void UpdateMyComp(MyComp_State state, MyComp_Props props);

void RenderComp(MyComp_State state, MyComp_Props props);
```

```c
MyComp_Private Init(MyComp_Props public);

void UpdateMyComp(MyComp_Private private, MyComp_Props public);

void RenderComp(MyComp_Private private, MyComp_Props public);
```


## Mixing pointers and value ?

```c
Launcher_State InitLauncher();

int UpdateLauncher(Launcher_State *state); // read/write function

void DrawLauncher(Launcher_State *state); // read only
```

```c
Launcher_State InitLauncher();

int UpdateLauncher(Launcher_State *state); // read/write function

void DrawLauncher(Launcher_State state); // read only
```

## Property Case  ?

```c
// Camelcase
struct LauncherState
{
    Texture2D texture;
    int durationInMs;
    clock_t clockAtStartup;
};
```

```c
// Snskecase
struct LauncherState
{
    Texture2D texture;
    int duration_in_ms;
    clock_t clock_at_startup;
};
```

```c
struct LauncherState
{
    Texture2D texture;
    int durationinms;
    clock_t clockatstartup;
};
```