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