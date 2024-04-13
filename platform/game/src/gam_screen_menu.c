#include "gam_screen_menu.h"

#include <raylib.h>
#include "gam_screens.h"

static int currentProcess = -1;
static int mouseHoverRec = -1;

static Rectangle BUTTON_MEDIUM = {
    0,
    0,
    150.0f, 40.0f};

static Rectangle BUTTON_SMALL = {
    0,
    0,
    30.0f, 40.0f};

static int FONT_SIZE_20 = 20;
static int FONT_SIZE_10 = 10;

void Init_Menu(AppConfiguration appConfig)
{
    currentProcess = -1;
    mouseHoverRec = -1;
}

int Update_Menu()
{
    // Mouse toggle group logic
    if (CheckCollisionPointRec(GetMousePosition(), BUTTON_MEDIUM))
    {
        mouseHoverRec = 1;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            currentProcess = 1;
            // textureReload = true;
        }
        // break;
    }
    else
        mouseHoverRec = -1;

    // quit
    if (IsKeyPressed(KEY_TAB))
    {
        return GAMEPLAY;
    }
    return MENU;
}

void Draw_Button(char *text, int posX, int posY, Rectangle buttonRec, int fontSize, Color color)
{
    const Color activeColor = SKYBLUE;
    // TODO: resolution scaling
    Rectangle rec = {
        posX,
        posY,
        buttonRec.width,
        buttonRec.height};
    Color backColor = ((1 == currentProcess) || (1 == mouseHoverRec)) ? activeColor : color;
    DrawRectangleRec(rec, backColor);
    // center text
    int textX = (int)(posX + rec.width / 2 - MeasureText(text, 10) / 2);
    int textY = (int)posY + 11;
    Color textColor = ((1 == currentProcess) || (1 == mouseHoverRec)) ? WHITE : BLACK;
    DrawText(text, textX, textY, fontSize, textColor);
}

void Draw_Menu()
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE); // Clear texture background

        // style
        const int SPACING = 30;
        const int LINE_HEIGHT = 60;
        const int LABEL_WIDTH = 200;
        const int BUTTON_SMALL_WIDTH = 200;
        const int COL_1 = 30;
        const int COL_1_0 = COL_1;
        const int COL_1_1 = COL_1 + 45;
        const int COL_1_2 = COL_1 + 90;
        const int COL_2 = 400;
        const int COL_2_1 = COL_2 + 70;
        const int COL_2_2 = COL_2 + 230;
        const int COL_3 = 700;
        const int CENTER = GetScreenWidth() / 2;
        const int CHAR_WIDTH = 10;

        // draw
        DrawText("Press [TAB] to toggle menu", 10, 10 + 30 * 0, 20, GRAY);
        int line = 0;
        DrawText("SETTINGS", CENTER - sizeof("SETTINGS") * CHAR_WIDTH, line, 40, GRAY);
        {
            line += LINE_HEIGHT;
            DrawText("DISPLAY", COL_1_1, line, FONT_SIZE_20, GRAY);
            {
                line += LINE_HEIGHT;
                DrawText("Window Mode:", COL_1_2, line, FONT_SIZE_20, GRAY);
                Draw_Button("<", COL_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
                DrawText("Windowed", COL_2_1, line, FONT_SIZE_20, GRAY);
                Draw_Button(">", COL_2_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
                line += LINE_HEIGHT;
                DrawText("Resolution:", COL_1_2, line, FONT_SIZE_20, GRAY);
                Draw_Button("<", COL_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
                DrawText("1280 x 720", COL_2_1, line, FONT_SIZE_20, GRAY);
                Draw_Button(">", COL_2_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
                line += LINE_HEIGHT;
                DrawText("Frame limit:", COL_1_2, line, FONT_SIZE_20, GRAY);
                Draw_Button("<", COL_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
                DrawText("60", COL_2_1, line, FONT_SIZE_20, GRAY);
                Draw_Button(">", COL_2_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
            }
            line += LINE_HEIGHT;
            DrawText("GRAPHICS", COL_1_1, line, FONT_SIZE_20, GRAY);
            {
                line += LINE_HEIGHT;
                DrawText("AntiAliasing:", COL_1_2, line, FONT_SIZE_20, GRAY);
                Draw_Button("<", COL_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
                DrawText("MSAA 4X", COL_2_1, line, FONT_SIZE_20, GRAY);
                Draw_Button(">", COL_2_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
                line += LINE_HEIGHT;
                DrawText("Postprocess:", COL_1_2, line, FONT_SIZE_20, GRAY);
                Draw_Button("<", COL_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
                DrawText("Bloom", COL_2_1, line, FONT_SIZE_20, GRAY);
                Draw_Button(">", COL_2_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
                DrawText("* Bloom breaks MSAA (no Anti-Aliasing)", COL_3, line, FONT_SIZE_20, GRAY);
                line += LINE_HEIGHT;
                DrawText("Lighting:", COL_1_2, line, FONT_SIZE_20, GRAY);
                Draw_Button("<", COL_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
                DrawText("On", COL_2_1, line, FONT_SIZE_20, GRAY);
                Draw_Button(">", COL_2_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
                DrawText("* Lighting breaks textures (darker)", COL_3, line, FONT_SIZE_20, GRAY);
            }
            line += LINE_HEIGHT;
            DrawText("CONTROLS", COL_1_1, line, FONT_SIZE_20, GRAY);
            {
                line += LINE_HEIGHT;
                DrawText("Kayboard Layout:", COL_1_2, line, FONT_SIZE_20, GRAY);
                Draw_Button("<", COL_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
                DrawText("QWERTY", COL_2_1, line, FONT_SIZE_20, GRAY);
                Draw_Button(">", COL_2_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
                line += LINE_HEIGHT;
                DrawText("Control Scheme:", COL_1_2, line, FONT_SIZE_20, GRAY);
                Draw_Button("<", COL_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
                DrawText("Tank", COL_2_1, line, FONT_SIZE_20, GRAY);
                Draw_Button(">", COL_2_2, line, BUTTON_SMALL, FONT_SIZE_20, LIGHTGRAY);
            }
        }
    }
    EndDrawing();
}

void Unload_Menu()
{
}