#include "eng_scene.h"
#include "eng_assets.h"

#include <stdio.h>
//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

Color Parse_ColorJson(const cJSON* src) {
    int r = cJSON_GetObjectItemCaseSensitive(src, "r")->valueint;
    return (Color) {
        r,
        cJSON_GetObjectItemCaseSensitive(src, "g")->valueint,
        cJSON_GetObjectItemCaseSensitive(src, "b")->valueint,
        cJSON_GetObjectItemCaseSensitive(src, "a")->valueint
    };
}

NodeTexture Parse_TextureJson(const cJSON* src) {
    cJSON* col1 = cJSON_GetObjectItemCaseSensitive(src, "col1");
    cJSON* col2 = cJSON_GetObjectItemCaseSensitive(src, "col2");
    NodeTexture tgt = (NodeTexture){ 0 };
    tgt.tilingX = cJSON_GetObjectItemCaseSensitive(src, "tilingX")->valueint;
    tgt.tilingY = cJSON_GetObjectItemCaseSensitive(src, "tilingY")->valueint;
    tgt.col1 = Parse_ColorJson(col1);
    tgt.col2 = Parse_ColorJson(col2);
    return tgt;
}

Transform2 Parse_Transform2Json(const cJSON *src) {
        cJSON* translation = cJSON_GetObjectItemCaseSensitive(src, "translation");
        //cJSON* rotation = cJSON_GetObjectItemCaseSensitive(src, "rotation");
        cJSON* scale = cJSON_GetObjectItemCaseSensitive(src, "scale");
        Transform2 tgt = (Transform2){
                (Vector3) {
                    cJSON_GetObjectItemCaseSensitive(translation, "x")->valueint,
                    cJSON_GetObjectItemCaseSensitive(translation, "y")->valueint,
                    cJSON_GetObjectItemCaseSensitive(translation, "z")->valueint
                },
                (Rotation2){
                    { 0.0f, 0.0f, 0.0f }, // TODO
                    ROTATE_ZERO}, // TODO
                (Vector3){
                    cJSON_GetObjectItemCaseSensitive(scale, "x")->valueint,
                    cJSON_GetObjectItemCaseSensitive(scale, "y")->valueint,
                    cJSON_GetObjectItemCaseSensitive(scale, "z")->valueint
                }
        };
        return tgt;
}

Node3D Parse_Node3dJson(const cJSON *json) {
    Node3D node3d = (Node3D) {0};
        strcpy(node3d.name, cJSON_GetObjectItemCaseSensitive(json, "name")->valuestring);
        strcpy(node3d.model, cJSON_GetObjectItemCaseSensitive(json, "model")->valuestring);
        node3d.transform = Parse_Transform2Json(cJSON_GetObjectItemCaseSensitive(json, "transform"));
        //node3d.color = Parse_ColorJson(cJSON_GetObjectItemCaseSensitive(json, "color")); // TODO: FIXME
        node3d.color = DARKGRAY;
        node3d.texture = Parse_TextureJson(cJSON_GetObjectItemCaseSensitive(json, "texture"));
        return node3d;
}

// test
cJSON* CreateNode3DJsonFile() {
    cJSON *json = cJSON_CreateObject();

        cJSON_AddNumberToObject(json, "id", 1);
        cJSON_AddStringToObject(json, "name", "SM_Cube4");
        cJSON_AddStringToObject(json, "model", "resources/models/SM_Cube.obj");
        // cJSON_AddStringToObject(json, "color", "DARKGRAY"); // TODO: FIXME

        cJSON* json_transform = cJSON_CreateObject();

            cJSON* json_tra2_trans = cJSON_CreateObject();
                cJSON_AddNumberToObject(json_tra2_trans, "x", 12);
                cJSON_AddNumberToObject(json_tra2_trans, "y", 0);
                cJSON_AddNumberToObject(json_tra2_trans, "z", 17);
            cJSON_AddItemToObject(json_transform, "translation", json_tra2_trans);

            cJSON* json_tra2_scale = cJSON_CreateObject(); 
                cJSON_AddNumberToObject(json_tra2_scale, "x", 60);
                cJSON_AddNumberToObject(json_tra2_scale, "y", 20);
                cJSON_AddNumberToObject(json_tra2_scale, "z", 50);
            cJSON_AddItemToObject(json_transform, "scale", json_tra2_scale);

        cJSON_AddItemToObject(json, "transform", json_transform);

        cJSON* json_color = cJSON_CreateObject(); 
            cJSON_AddNumberToObject(json_color, "r", 80);
            cJSON_AddNumberToObject(json_color, "g", 80);
            cJSON_AddNumberToObject(json_color, "b", 80);
            cJSON_AddNumberToObject(json_color, "a", 255);
        cJSON_AddItemToObject(json, "color", json_color);

        cJSON* json_texture = cJSON_CreateObject(); 
            cJSON_AddNumberToObject(json_texture, "tilingX", 2);
            cJSON_AddNumberToObject(json_texture, "tilingY", 5);

            cJSON* json_texture_col1 = cJSON_CreateObject(); 
                cJSON_AddNumberToObject(json_texture_col1, "r", 80);
                cJSON_AddNumberToObject(json_texture_col1, "g", 80);
                cJSON_AddNumberToObject(json_texture_col1, "b", 80);
                cJSON_AddNumberToObject(json_texture_col1, "a", 255);
             cJSON_AddItemToObject(json_texture, "col1", json_texture_col1);

            cJSON* json_texture_col2 = cJSON_CreateObject(); 
                cJSON_AddNumberToObject(json_texture_col2, "r", 130);
                cJSON_AddNumberToObject(json_texture_col2, "g", 130);
                cJSON_AddNumberToObject(json_texture_col2, "b", 130);
                cJSON_AddNumberToObject(json_texture_col2, "a", 255);
            cJSON_AddItemToObject(json_texture, "col2", json_texture_col2);

        cJSON_AddItemToObject(json, "texture", json_texture);

        return json;
}

cJSON* Read_SceneJsonFile(const char *fileName) {
    char tmp[PATH_MAX];
    char* path = GetAssetPath(tmp, fileName);
    // open the file
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return 1;
    }
    // read the file contents into a string
    char buffer[1024];
    int len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);
    // parse the JSON data
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("Error: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        return 1;
    }
    return json;
    // return CreateNode3DJsonFile(); // test
}

Node3D* Parse_SceneJson(const cJSON *json) {
    Node3D nodes[1] = {0};
    cJSON* json_nodes = cJSON_GetObjectItemCaseSensitive(json, "nodes");
    cJSON* json_i = NULL;
    cJSON_ArrayForEach(json_i, json_nodes)
    {
        nodes[0] = Parse_Node3dJson(json_i);
    }
    return nodes;
}
