#include <raylib.h>

#pragma once

typedef struct Rotation2
{
    Vector3 axis;
    float angle;
} Rotation2;

typedef struct Transform2
{
    Vector3 translation;
    Rotation2 rotation; // Quaternion rotation;
    Vector3 scale;
} Transform2;

typedef struct GameObject
{
    char name[256];
    Transform2 transform;
    Model model;
    Color color;
    //Material2 material;
    // TODO: Resources
    // TODO: Instances
} GameObject;

typedef enum
{
    ROTATE_ZERO = 0,
    ROTATE_P90 = 90,
    ROTATE_M90 = -90,
    ROTATE_M180 = -180,
    ROTATE_M270 = -270,
} Rotate;

static const Vector3 ROTATION_YAW = {0, 1, 0};

void Draw_GameObject(GameObject go);

//void Init_Models(GameObject *go_arr);
//
//void Init_Model(GameObject go);