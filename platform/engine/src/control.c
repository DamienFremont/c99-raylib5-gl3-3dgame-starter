#include "control.h"

void TankControl_Move(Vector3 *playerPosition, InputActionValue value, float char_speed)
{
    playerPosition->z = playerPosition->z + char_speed * value.Axis2D.x;
    playerPosition->x = playerPosition->x + char_speed * value.Axis2D.y;
    // TODO: forward and rotation
}
