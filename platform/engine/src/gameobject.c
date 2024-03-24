#include "gameobject.h"

#include <raylib.h>

void Draw_Component(GameObject go)
{
    DrawModelEx(go.model,
                go.transform.translation,
                go.transform.rotation.axis,
                go.transform.rotation.angle,
                go.transform.scale,
                go.color);
}

//void Init_Models(GameObject *go_arr)
//{
//    for (int i = 0; i < sizeof(go_arr); i++)
//        Init_Model(go_arr[i]);
//}
//
//void Init_Model(GameObject go)
//{
//    Model model = go.model;
//    if (go.material.type == MATERIAL2_TEXTURE)
//    {
//        Texture2D texture = go.material.texture;
//        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
//    }
//}
