#include "gameobject.h"

#include <raylib.h>

void Draw_ComponentColor(StaticMeshComponent comp)
{
    DrawModelEx(comp.staticMesh,
                comp.transform.translation,
                comp.transform.rotation.axis,
                comp.transform.rotation.angle,
                comp.transform.scale,
                comp.color);
}

void Draw_Component(StaticMeshComponent comp)
{
    Model model = comp.staticMesh;
    if (comp.material.type == MATERIAL2_TEXTURE)
    {
        Texture2D texture = comp.material.texture;
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    }
    else if (comp.material.type == MATERIAL2_TEXTURESHADER)
    {
        Texture2D texture = comp.material.texture;
        Shader shader = comp.material.shader;
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
        model.materials[0].shader = shader;
    }
    Draw_ComponentColor(comp);
}
