#include "eng_scene.h"

#include "eng_json.h"
#include "eng_scene_json.h"

//---------------------------------------------------------
// Module specific Functions Definition
//---------------------------------------------------------

Scene LoadSceneFile(const char *fileName)
{
    cJSON* json = Read_JsonFile("resources/scenes/unrealthirdperson.json");
    Scene scene = Parse_SceneJson(json);
    cJSON_Delete(json);
    return scene;
}
