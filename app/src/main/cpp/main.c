/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press Shift+F10 for Android Studio.
*
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5) and reviewed by Victor Le Juez
*
********************************************************************************************/

#include <math.h>
#include "raymob.h" // This header can replace 'raylib.h' and includes additional functions related to Android.

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
#define MAX_BRUSH_SIZE 35

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(0, 0, "Complex Graph");

    SetTargetFPS(120);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    RenderTexture2D Canvas = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    
    /* Texture2D clearButton = LoadTexture("../assets/icons/close.png"); */
    Texture2D clearButton = LoadTexture("icons/close.png");
    Rectangle clearButtonRect = (Rectangle) {50, 50, clearButton.width * 0.06, clearButton.height*0.06};

    Texture2D menuButton = LoadTexture("icons/menu.png");
    Rectangle menuButtonRect = (Rectangle) {GetScreenWidth() - (50 + menuButton.width*0.06), 50, menuButton.width * 0.06, clearButton.height*0.06};
    Rectangle menuRect = (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()/6};
    Rectangle scrollBar = (Rectangle) {(menuRect.width - menuRect.width/2)/2, menuRect.height/1.5, menuRect.width/2, 30};
    bool isMenuOpen = false;
    
    float brushSize = 15;
    
    Vector2 mousePrevPos = {0, 0};
    int mouseTimer = 0;

    BeginTextureMode(Canvas);
        ClearBackground(WHITE);
    EndTextureMode();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        mouseTimer++;
        Vector2 mousePos = GetMousePosition();
        if (mousePrevPos.x == 0 && mousePrevPos.y == 0)  mousePrevPos = mousePos;

        if (CheckCollisionRecs(clearButtonRect, (Rectangle) {mousePos.x, mousePos.y, 30, 30}))
        {
            BeginTextureMode(Canvas);
                ClearBackground(WHITE);
            EndTextureMode();
        }

        else if (IsMouseButtonDown(0) && CheckCollisionRecs(scrollBar, (Rectangle) {mousePos.x, mousePos.y, 30, 30}) && isMenuOpen)
        {
            brushSize = ((mousePos.x - scrollBar.x) / scrollBar.width) * MAX_BRUSH_SIZE + 1;
        }
        else if (IsMouseButtonPressed(0) && CheckCollisionRecs(menuButtonRect, (Rectangle) {mousePos.x, mousePos.y, 30, 30}))
        {
            isMenuOpen = !isMenuOpen;
        }
        else{
            if (IsMouseButtonDown(0) || GetGestureDetected() == GESTURE_DRAG)
            {
                if (!(isMenuOpen && mousePos.y <= menuRect.height ||  CheckCollisionRecs(menuButtonRect, (Rectangle) {mousePos.x, mousePos.y, 30, 30})))
                {
                    BeginTextureMode(Canvas);
                        float distance = sqrt((mousePos.x - mousePrevPos.x)*(mousePos.x - mousePrevPos.x) + (mousePos.y - mousePrevPos.y)*(mousePos.y - mousePrevPos.y));
                        // fill the gaps
                        if (mouseTimer <= 5) 
                        {
                            for (int i = 1, steps = distance*10 / brushSize; i < steps; i++)
                            {
                                DrawCircle(mousePrevPos.x + (mousePos.x - mousePrevPos.x) * i/steps , mousePrevPos.y + (mousePos.y - mousePrevPos.y) * i/steps , brushSize, BLACK);
                            }
                        }
                        else {
                            DrawCircle((int)mousePos.x , (int)mousePos.y, brushSize, BLACK);
                        }
                        mouseTimer = 0;
                    EndTextureMode();
                }
            }
        }
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            DrawTextureRec(Canvas.texture, (Rectangle) {0, 0, GetScreenWidth(), -GetScreenHeight()}, (Vector2) {0, 0}, WHITE);
            if (isMenuOpen) {
                DrawRectangleRec(menuRect, (Color) {237, 237, 237, 180});
                DrawRectangleRec(scrollBar, GRAY);
                DrawRectangle(scrollBar.x, scrollBar.y, menuRect.width/2*(brushSize / MAX_BRUSH_SIZE) , scrollBar.height, RED);
            }
            DrawTextureEx(clearButton, (Vector2){clearButtonRect.x, clearButtonRect.y}, 0, 0.06, WHITE);
            DrawTextureEx(menuButton, (Vector2){menuButtonRect.x, menuButtonRect.y}, 0, 0.06, WHITE);
        EndDrawing();
        //----------------------------------------------------------------------------------
        mousePrevPos = mousePos;
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    UnloadTexture(clearButton);
    //--------------------------------------------------------------------------------------

    return 0;
}
