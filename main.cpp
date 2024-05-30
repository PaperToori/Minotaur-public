#include <stdio.h>
#include <math.h>
#include "/opt/homebrew/Cellar/raylib/4.5.0/include/raylib.h"
#include "/opt/homebrew/Cellar/raylib/4.5.0/include/raymath.h"
#include "mylib.h"
#include "textures.cpp"
#include "display.cpp"
#include "input.cpp"
#include "update.cpp"

int main(void)
{
    // Gamestate
    GameState gamestate = menu;
    short points;

    // Timers
    float levelTimer = LEVEL_TIMER;
    float scoreTimer = 0;

    // Charcter
    Vector2 pos = (Vector2){300, 300};
    Vector2 cam = (Vector2){0, 0};
    Vector2 mov = (Vector2){0, 0};
    int vol = 200;
    ViewState view = head;
    float rotation = 0.0;

    // Rays
    Laser cast[RAYCOUNT];

    // arr is map grid
    short arr[MAP_WIDTH][MAP_HEIGHT];
    short displayedMap[MAP_WIDTH][MAP_HEIGHT];

    // This sets the stage
    D_Init();
    M_Init(arr,
           displayedMap,
           &pos,
           &points);
    GenerateMaze(arr,
                 displayedMap,
                 pos);

    while (!WindowShouldClose())
    {
        switch (gamestate)
        {
        case menu:
        {
            // Input
            I_Menu(&gamestate);

            // Draw
            D_Menu();
        }
        break;
        case game:
        {
            // Input
            I_Game(&mov,
                   &rotation,
                   vol,
                   &view);

            // Update
            U_Game(arr,
                   displayedMap,
                   cast,
                   &pos,
                   &cam,
                   mov,
                   rotation,
                   &points,
                   &scoreTimer,
                   &levelTimer,
                   &gamestate);

            // Draw
            D_Game(view,
                   arr,
                   displayedMap,
                   pos,
                   cam,
                   cast,
                   rotation,
                   points,
                   scoreTimer,
                   levelTimer);
        }
        break;
        case deathscreen:
        {
            // Input
            I_DeathScreen(&gamestate,
                          &points);

            // Draw
            D_DeathScreen(points);
        }
        break;
        }
    }

    return 0;
}
