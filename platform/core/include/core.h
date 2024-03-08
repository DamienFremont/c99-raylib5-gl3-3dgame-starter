#include <stdbool.h>

typedef struct AppProperties AppProperties;

struct AppProperties
{
    char res_path[999];
    bool msaa_enable;
    bool bloom_enable;
    int glsl_version; // 330 desktop
    int fps_cap; // 0 = Unlimited
    bool showFPS;
};

int main(AppProperties appProps);
