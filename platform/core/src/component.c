#include "component.h"

#include <raylib.h>

void Draw_Component(StaticMeshComponent comp)
{
    DrawModelEx(comp.staticMesh,
        comp.transform.translation,
        comp.transform.rotation.axis,
        comp.transform.rotation.angle,
        comp.transform.scale,
        comp.color);
}