#include <raylib.h>

#pragma once

typedef enum
{
    MATERIAL2_COLOR = 0,
    MATERIAL2_TEXTURE = 1,
    MATERIAL2_TEXTURESHADER = 2,
} Material2Type;

typedef struct Material2 Material2;
typedef struct Material2
{
    Material2Type type;
    Texture2D texture;
    Shader shader;
};

typedef struct Rotation2 Rotation2;
typedef struct Rotation2
{
    Vector3 axis;
    float angle;
};

typedef struct Transform2 Transform2;
typedef struct Transform2
{
    Vector3 translation;
    Rotation2 rotation; // Quaternion rotation;
    Vector3 scale;
};

typedef struct StaticMeshComponent StaticMeshComponent;
struct StaticMeshComponent
{
    char name[256];
    Transform2 transform;
    Model staticMesh;
    Color color;
    Material2 material;
};

typedef enum
{
    ROTATE_ZERO = 0,
    ROTATE_P90 = 90,
    ROTATE_M90 = -90,
    ROTATE_M180 = -180,
} Rotate;

Material LoadMaterialResource(Texture2D texture, Shader shader);

void Draw_Component(StaticMeshComponent comp);
