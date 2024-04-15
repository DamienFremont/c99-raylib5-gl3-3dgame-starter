#include "eng_camera.h"

#include <raylib.h>

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

Camera InitCamera()
{
    Camera camera = {0};
    camera.position = (Vector3){0.5f, 0.5f, 1.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.25f, 0.0f};  // Camera looking at point
    camera.up = (Vector3){0.0f, 0.65f, 0.0f};      // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                           // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;        // Camera projection type
    return camera;
}

void CameraFixed_Look(Camera *camera, Controller player, Vector3 cameraTransform)
{
    camera->position = (Vector3){
        player.position.x - (player.direction.x * cameraTransform.z),
        player.position.y + cameraTransform.y,
        player.position.z - (player.direction.z * cameraTransform.z)};
    camera->target = (Vector3){
        player.position.x,
        player.position.y + cameraTransform.y,
        player.position.z};
}

void CameraFollow_Look(Camera *camera, Controller player, Vector3 cameraTransform)
{
    // TODO
}
