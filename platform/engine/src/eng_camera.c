#include "eng_camera.h"

#include <raylib.h>

Vector3 cameraDistance;

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

Camera InitCamera(float fovy, Vector3 distance)
{
    cameraDistance = distance;
    Camera camera = {0};
    camera.position = (Vector3){0.0f, 0.0f, 0.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};  // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};       // Camera up vector (rotation towards target)
    camera.fovy = fovy;                             // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;        // Camera projection type
    return camera;
}

void CameraSecondPerson_Look(Camera *camera, Controller player)
{
    const Vector3 target = player.position;
    // camera->position = camera->position;
    camera->target = (Vector3){
        target.x,
        target.y + cameraDistance.y,
        target.z};
}

void CameraThirdPerson_Look(Camera *camera, Controller player)
{
    const Vector3 target = player.position;
    camera->position = (Vector3){
        target.x - (player.direction.x * cameraDistance.z),
        target.y + cameraDistance.y,
        target.z - (player.direction.z * cameraDistance.z)};
    camera->target = (Vector3){
        target.x,
        target.y + cameraDistance.y,
        target.z};
}

void CameraFollow_Look(Camera *camera, Controller player)
{
    // TODO
}
