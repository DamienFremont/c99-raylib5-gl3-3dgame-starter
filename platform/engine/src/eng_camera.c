#include "eng_camera.h"

#include <raylib.h>
#include <raymath.h>

Vector3 cameraDistance;

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

Camera InitCamera(float fovy, Vector3 distance)
{
    cameraDistance = distance;
    Camera cam = {0};
    cam.position = (Vector3){0.0f, 0.0f, 0.0f}; // Camera position
    cam.target = (Vector3){0.0f, 0.0f, 0.0f};   // Camera looking at point
    cam.up = (Vector3){0.0f, 1.0f, 0.0f};       // Camera up vector (rotation towards target)
    cam.fovy = fovy;                            // Camera field-of-view Y
    cam.projection = CAMERA_PERSPECTIVE;        // Camera projection type
    return cam;
}

void CameraSecondPerson_Look(Camera *camera, Controller player)
{
    const Vector3 tgt = player.position;
    // camera->position // DO NOTHING
    camera->target = (Vector3){
        tgt.x,
        tgt.y + cameraDistance.y,
        tgt.z};
}

void CameraThirdPerson_Look(Camera *camera, Controller player)
{
    const Vector3 tgt = player.position;
    camera->position = (Vector3){
        tgt.x - (player.direction.x * cameraDistance.z),
        tgt.y + cameraDistance.y,
        tgt.z - (player.direction.z * cameraDistance.z)};
    camera->target = (Vector3){
        tgt.x,
        tgt.y + cameraDistance.y,
        tgt.z};
}

void CameraFollow_Look(Camera *camera, Controller player)
{
    Vector3 tgt = player.position;
    Vector3 pos = camera->position;
    float dist = Vector3Distance(pos, tgt);
    if (dist > cameraDistance.z)
    {
        float diff = cameraDistance.z / dist;
        Vector3 diff_vect = (Vector3){
            (tgt.x - pos.x) * diff,
            -cameraDistance.y,
            (tgt.z - pos.z) * diff};
        camera->position = Vector3Subtract(tgt, diff_vect);
    }
    camera->target = (Vector3){
        tgt.x,
        tgt.y + cameraDistance.y,
        tgt.z};
}
