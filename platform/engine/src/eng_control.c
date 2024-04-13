#include "eng_control.h"

#include <raymath.h>

void TankControl_Move(Controller *player, InputActionValue value, float char_speed, float char_rot)
{
    float new_step = (char_speed) * (value.Axis2D.y);
    float new_angle = - (char_rot) * (value.Axis2D.x);
    Vector3 new_dir = Vector3RotateByAxisAngle(player->direction, ROTATION_YAW, DEG2RAD * new_angle);
    Vector3 new_dis = Vector3Scale(new_dir, new_step);
    Vector3 new_pos = Vector3Add(player->position, new_dis);
    player->direction = new_dir;
    player->position = new_pos;
}

float TankControl_ModelRotationAngle(Vector3 modelDirection)
{
    const Vector2 LEVEL_DIRECTION_2D = {0, 1};
    Vector2 dir2D = {
        modelDirection.x,
        modelDirection.z};
    float rad = Vector2Angle(LEVEL_DIRECTION_2D, dir2D);
    float deg = RAD2DEG * rad;
    return -deg;
}
