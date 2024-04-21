#include "eng_gameobject.h"

#include <raylib.h>
#include "eng_text.h"

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

const Vector3 PLAYER_HITBOX_BOUNDARY = {1.0f, 2.0f, 1.0f};

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

void Draw_GameObject(GameObject go)
{
    DrawModelEx(go.model,
                go.transform.translation,
                go.transform.rotation.axis,
                go.transform.rotation.angle,
                go.transform.scale,
                go.color);
}

void Draw_GameObject_Console(GameObject go)
{
    Vector3 trans = go.transform.translation;
    Vector3 scale = go.transform.scale;
    // name
    DrawText3D(GetFontDefault(), go.name, (Vector3){                 //
                                                    trans.x,         //
                                                    trans.y + 0.01f, //
                                                    trans.z + 0.5f},
               FONT_SIZE_12, FONT_SPACING_1, FONT_LINE_SPACING_1, BACKFACE_FALSE, WHITE);
    // TODO: physics
    DrawCubeWiresV((Vector3){
                       trans.x + scale.x / 2,
                       trans.y + scale.y / 2,
                       trans.z + scale.z / 2},
                   (Vector3){scale.x, scale.y, scale.z}, GREEN);
}

void Draw_GameObject_PlayerHitBox(GameObject go)
{
    Vector3 trans = go.transform.translation;
    // player hitbox
    DrawCubeWiresV((Vector3){
                       trans.x,
                       trans.y + 1.0f,
                       trans.z},
                   PLAYER_HITBOX_BOUNDARY, RED);
}
