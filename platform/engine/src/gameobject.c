#include "gameobject.h"

#include <raylib.h>

void Draw_GameObject(GameObject go)
{
    DrawModelEx(go.model,
                go.transform.translation,
                go.transform.rotation.axis,
                go.transform.rotation.angle,
                go.transform.scale,
                go.color);
}
