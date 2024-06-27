
//---------------------------------------------------------
// Types and Structures Definition
//---------------------------------------------------------

typedef enum
{
    NATIVE = 0,
    CGA,
    SD,
    HD,
    FHD,
    QHD,
    UHD,
} ScreenResolutionType;

//---------------------------------------------------------
// Module Functions Declaration
//---------------------------------------------------------

int ENG_GetScreenWidth(ScreenResolutionType type);
int ENG_GetScreenHeight(ScreenResolutionType type);
