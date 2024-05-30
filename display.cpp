#include "mylib.h"

void D_Init()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minotaur");
    SetWindowPosition(60, 60);
}

void D_Menu()
{
    BeginDrawing();

    ClearBackground(WHITE);

    DrawText("Minotaur",
             (SCREEN_WIDTH / 2) - 140,
             (SCREEN_HEIGHT / 2) - 120,
             60,
             DARKGRAY);
    DrawText("Press 'Space' to start",
             (SCREEN_WIDTH / 2) - 250,
             (SCREEN_HEIGHT / 2) + 30,
             40,
             DARKGRAY);
    DrawText("A & D             - Strafe",
             40,
             SCREEN_HEIGHT - 125,
             20,
             DARKGRAY);
    DrawText("Up & DOWN      - Move",
             40,
             SCREEN_HEIGHT - 100,
             20,
             DARKGRAY);
    DrawText("LEFT & RIGHT - Rotate",
             40,
             SCREEN_HEIGHT - 75,
             20,
             DARKGRAY);
    DrawText("M (hold)          - Map",
             40,
             SCREEN_HEIGHT - 50,
             20,
             DARKGRAY);
    EndDrawing();
}

void D_DeathScreen(short points)
{
    BeginDrawing();
    ClearBackground(WHITE);
    DrawText("You're hella' dead.",
             SCREEN_WIDTH / 2 - 200,
             SCREEN_HEIGHT - 500,
             40,
             BLACK);
    DrawText("Wanna try again? [SPACE]",
             SCREEN_WIDTH / 2 - 200,
             SCREEN_HEIGHT - 200,
             40,
             BLACK);
    DrawText(TextFormat("Score: %i", points),
             SCREEN_WIDTH / 2 - 200,
             SCREEN_HEIGHT - 400,
             40,
             DARKGRAY);
    EndDrawing();
}

void D_Game(ViewState view,
            short arr[MAP_WIDTH][MAP_HEIGHT],
            short displayedMap[MAP_WIDTH][MAP_HEIGHT],
            Vector2 pos,
            Vector2 cam,
            Laser *cast,
            float rotation,
            short points,
            float scoreTimer,
            float levelTimer)
{
    BeginDrawing();

    switch (view)
    {
        // Top-Down Perspective
    case bird:
    {
        ClearBackground(DARKGRAY);
        // Objects / Walls
        for (int i = 0; i < MAP_WIDTH; i++)
        {
            for (int j = 0; j < MAP_HEIGHT; j++)
            {
                if (1 == arr[i][j] && 1 == displayedMap[i][j])
                {
                    DrawRectangle(
                        (i * MAP_GRID_SQUARE_SIZE) + MAP_DISPLAY_OFFSET_X,
                        (j * MAP_GRID_SQUARE_SIZE) + MAP_DISPLAY_OFFSET_Y,
                        MAP_GRID_SQUARE_SIZE,
                        MAP_GRID_SQUARE_SIZE,
                        LIGHTGRAY);
                }
                else if (arr[i][j] == 2)
                {
                    DrawRectangle(
                        (i * MAP_GRID_SQUARE_SIZE) + MAP_DISPLAY_OFFSET_X,
                        (j * MAP_GRID_SQUARE_SIZE) + MAP_DISPLAY_OFFSET_Y,
                        MAP_GRID_SQUARE_SIZE,
                        MAP_GRID_SQUARE_SIZE,
                        GREEN);
                }
            }
        }
        // Player
        Vector2 mapPos = (Vector2){(pos.x / 25) * MAP_GRID_SQUARE_SIZE + MAP_DISPLAY_OFFSET_X,
                                   (pos.y / 25) * MAP_GRID_SQUARE_SIZE + MAP_DISPLAY_OFFSET_Y};
        Vector2 mapCam = (Vector2){(cam.x / 25) * MAP_GRID_SQUARE_SIZE + MAP_DISPLAY_OFFSET_X,
                                   (cam.y / 25) * MAP_GRID_SQUARE_SIZE + MAP_DISPLAY_OFFSET_Y};
        DrawCircleV(mapPos, 3.0, ORANGE);
        DrawLineEx(mapPos, mapCam, 3.0, ORANGE);
    }
    break;

        // Player's Perspective
    case head:
    {
        ClearBackground(BLACK);

        float wallTop;
        float wallBottom;
        float height = 0.0;

        Colour colour = RED;

        int wallX = 0;
        int wallY = 0;
        int wallRelevant = 0;

        for (int k = 0; k < RAYCOUNT; k++)
        {
            // Calculating distance and height of walls
            height = 10000.0 / cast[k].length;
            wallTop = SCREEN_HEIGHT * 0.5 - height;
            wallBottom = SCREEN_HEIGHT * 0.5 + height;

            // Checks distance on block
            wallX = (int)cast[k].edge.x % CUBE_WIDTH;
            wallY = (int)cast[k].edge.y % CUBE_WIDTH;

            // Uses "Distance on block" to give the correct texture
            if (wallX == 0 || wallX == TEXTURE_SIZE - 1)
            {
                wallRelevant = wallY;
            }
            else
            {
                wallRelevant = wallX;
            }

            // Draw player view: first sky, then wall, then floor.
            DrawLine(k, 0, k, wallTop, (Colour){50, 35, 50, 255});

            for (int d = 0; d < TEXTURE_HEIGHT; d++)
            {
                colour = (Colour){0,
                                  (unsigned char)(55 + 100 * wallTexture[wallRelevant + TEXTURE_SIZE * d]),
                                  70,
                                  255};

                DrawLine(k, wallTop + (height * 2 / TEXTURE_HEIGHT) * d,
                         k, wallTop + (height * 2 / TEXTURE_HEIGHT) * (d + 1),
                         colour);
            }

            // If goal was spotted, draw this instead of wall
            if (1 == cast[k].spotted)
            {
                DrawLine(k, wallTop,
                         k, wallBottom,
                         GREEN);
            }

            DrawLine(k, wallBottom, k, SCREEN_HEIGHT, (Colour){50, 35, 50, 255});
        }
    }
    break;
    }
    // Useful Data
    if (0 < scoreTimer)
    {
        DrawText(TextFormat("%i", points),
                 40,
                 40,
                 300,
                 YELLOW);
    }
    DrawText(TextFormat("FPS: %03i", GetFPS()),
             SCREEN_WIDTH - 200,
             SCREEN_HEIGHT - 70,
             20,
             DARKGRAY);
    DrawText(TextFormat("TIME: %04f", levelTimer),
             20,
             20,
             40,
             RED);

    EndDrawing();
}