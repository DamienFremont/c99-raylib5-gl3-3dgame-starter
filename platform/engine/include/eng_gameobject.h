#pragma once
#include <raylib.h>

//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

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
    ANGLE_ZERO = 0,
    ANGLE_P90 = 90,
    ANGLE_M90 = -90,
    ANGLE_M180 = -180,
    ANGLE_M270 = -270,
} Angle;

static const Vector3 AXIS_YAW = {0, 1, 0};

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

void DrawGameObject(GameObject go);

void DrawConsoleGameObject(GameObject go);
void DrawConsolePlayerHitBox(GameObject go);
