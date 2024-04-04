#include "control.h"

#include <raymath.h>

void TankControl_Move(Controller *player, InputActionValue value, float char_speed, float char_rot)
{
    float new_step = (char_speed) * (value.Axis2D.y);
    float new_angle = - (char_rot) * (value.Axis2D.x);
    Vector3 new_dir = Vector3RotateByAxisAngle(player->direction, ROTATION_YAW, DEG2RAD * new_angle);

    Vector3 new_dis = Vector3Scale(new_dir, new_step);
    //Vector3 new_dis = {(char_speed) * (value.Axis2D.y),0,(char_speed) * (value.Axis2D.x)};
    
    Vector3 new_pos = Vector3Add(player->position, new_dis);
    player->direction = new_dir;
    player->position = new_pos;
}
