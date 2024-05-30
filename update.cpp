#include "/opt/homebrew/Cellar/raylib/4.5.0/include/raylib.h"
#include "/opt/homebrew/Cellar/raylib/4.5.0/include/raymath.h"
#include "mylib.h"

void U_Game(short arr[MAP_WIDTH][MAP_HEIGHT],
            short displayedMap[MAP_WIDTH][MAP_HEIGHT],
            Laser *cast,
            Vector2 *pos,
            Vector2 *cam,
            Vector2 mov,
            float rotation,
            short *points,
            float *scoreTimer,
            float *levelTimer,
            GameState* gamestate)
{
    // Player movement
    float movX = (mov.x * GetFrameTime());
    float movY = (mov.y * GetFrameTime());

    (*pos).x += movX;
    (*pos).y += movY;

    // Player Map Collision (I am well aware that this is an abomination, but it works... for now)
    // Future note: Reuse collition optimisation for raycasting to minimise collision checks
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_HEIGHT; j++)
        {
            if (arr[i][j] == IS_WALL)
            {
                if ((*pos).y > j * CUBE_WIDTH - BODY_RAD && (*pos).y < j * CUBE_WIDTH + CUBE_WIDTH + BODY_RAD)
                {
                    if ((*pos).x > i * CUBE_WIDTH - BODY_RAD && (*pos).x < i * CUBE_WIDTH + CUBE_WIDTH + BODY_RAD)
                    {
                        (*pos).x -= movX;
                    }
                }
                if ((*pos).x > i * CUBE_WIDTH - BODY_RAD && (*pos).x < i * CUBE_WIDTH + CUBE_WIDTH + BODY_RAD)
                {
                    if ((*pos).y > j * CUBE_WIDTH - BODY_RAD && (*pos).y < j * CUBE_WIDTH + CUBE_WIDTH + BODY_RAD)
                    {
                        (*pos).y -= movY;
                    }
                }
            }
        }
    }

    // Check if player is in contaxt with goal
    if (IS_GOAL == arr[(int)(*pos).x / CUBE_WIDTH][(int)(*pos).y / CUBE_WIDTH])
    {
        // Move player into square - avoid getting stuck in wall
        (*pos).x = (int)((*pos).x / CUBE_WIDTH) * CUBE_WIDTH + BODY_RAD * 1.5;
        (*pos).y = (int)((*pos).y / CUBE_WIDTH) * CUBE_WIDTH + BODY_RAD * 1.5;

        (*points)++;
        *scoreTimer = SCORE_DISPLAY_TIMER;

        GenerateMaze(arr, displayedMap, *pos);
    }

    // Updates bird-eye map with local grid
    displayedMap[((int)(*pos).x / CUBE_WIDTH) - 1][((int)(*pos).y / CUBE_WIDTH) - 1] = 1;
    displayedMap[((int)(*pos).x / CUBE_WIDTH)][((int)(*pos).y / CUBE_WIDTH) - 1] = 1;
    displayedMap[((int)(*pos).x / CUBE_WIDTH) + 1][((int)(*pos).y / CUBE_WIDTH) - 1] = 1;
    displayedMap[((int)(*pos).x / CUBE_WIDTH) - 1][((int)(*pos).y / CUBE_WIDTH)] = 1;
    displayedMap[((int)(*pos).x / CUBE_WIDTH)][((int)(*pos).y / CUBE_WIDTH)] = 1;
    displayedMap[((int)(*pos).x / CUBE_WIDTH) + 1][((int)(*pos).y / CUBE_WIDTH)] = 1;
    displayedMap[((int)(*pos).x / CUBE_WIDTH) - 1][((int)(*pos).y / CUBE_WIDTH) + 1] = 1;
    displayedMap[((int)(*pos).x / CUBE_WIDTH)][((int)(*pos).y / CUBE_WIDTH) + 1] = 1;
    displayedMap[((int)(*pos).x / CUBE_WIDTH) + 1][((int)(*pos).y / CUBE_WIDTH) + 1] = 1;

    // player "nose" (seen on map)
    (*cam).x = (*pos).x + 10 * cosf(rotation);
    (*cam).y = (*pos).y + 10 * sinf(rotation);

    // Raycast Init
    Vector2 bearth = Vector2Zero();

    for (int k = 0; k < RAYCOUNT; k++)
    {
        bearth = Vector2Rotate((Vector2){0, (float)(-1 * viewGirth + (k * (viewGirth / (RAYCOUNT * 0.5))))},
                               rotation);

        cast[k].start.x = (*pos).x + bearth.x;
        cast[k].edge.x = cast[k].start.x;
        cast[k].start.y = (*pos).y + bearth.y;
        cast[k].edge.y = cast[k].start.y;
    }

    float angleDiff;
    // Raycast collisions
    for (int k = 0; k < RAYCOUNT; k++)
    {
        cast[k].spotted = 0;

        cast[k].angle = rotation -
                        ((visionCone * 0.5) -
                         (visionCone / (2 * RAYCOUNT))) +
                        ((visionCone / RAYCOUNT) * k);

        while (true)
        {
            cast[k].x = (int)(cast[k].edge.x / CUBE_WIDTH);
            cast[k].y = (int)(cast[k].edge.y / CUBE_WIDTH);
            if (IS_WALL == arr[cast[k].x][cast[k].y])
            {
                break;
            }
            else if (IS_GOAL == arr[cast[k].x][cast[k].y])
            {
                cast[k].spotted = 1;
                break;
            }
            cast[k].edge.x += cosf(cast[k].angle) * CUBE_WIDTH * stepSize;
            cast[k].edge.y += sinf(cast[k].angle) * CUBE_WIDTH * stepSize;
        }

        // Fisheye Correction
        angleDiff = rotation - cast[k].angle;
        if (angleDiff < 0)
        {
            angleDiff += PI * 2;
        }
        else if (angleDiff > 2 * PI)
        {
            angleDiff -= PI * 2;
        }
        cast[k].length = cosf(angleDiff) * Vector2Distance(*pos, cast[k].edge);
    }

    // Level timer 0 = bad
    *levelTimer -= GetFrameTime();
    *scoreTimer -= GetFrameTime();
    if (0 > *levelTimer){
        *gamestate = deathscreen;
        *levelTimer = LEVEL_TIMER;
        *scoreTimer = 0;
    }
}