#include "component.h"

#include <raylib.h>

void Draw_Component(StaticMeshComponent comp)
{

    Model model = comp.staticMesh;
    Vector3 position = comp.transform.translation;
    Vector3 rotationAxis = comp.transform.rotation.axis;
    float rotationAngle = comp.transform.rotation.angle;
    Vector3 scale = comp.transform.scale;
    Color color = comp.color;
    // DrawModelEx(model, position, rotationAxis, rotationAngle, scale, color);
    DrawModel(model, position, 0.0f, color);
}