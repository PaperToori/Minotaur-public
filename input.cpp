#include "/opt/homebrew/Cellar/raylib/4.5.0/include/raylib.h"
#include "mylib.h"

void I_Menu(GameState *gamestate)
{
    if (IsKeyPressed(KEY_SPACE))
    {
        *gamestate = game;
    }
    return;
}
void I_DeathScreen(GameState *gamestate,
                   short *points)
{
    if (IsKeyPressed(KEY_SPACE))
    {
        *gamestate = menu;
        *points = 0;
    }
}

void I_Game(Vector2 *mov,
            float *rotation,
            int vol,
            ViewState *view)
{
    (*mov).x = 0;
    (*mov).y = 0;

    if (IsKeyDown(KEY_M))
    {
        *view = bird;
        return;
    }
    else
    {
        *view = head;
    }

    if (IsKeyDown(KEY_UP))
    {
        (*mov).y = vol * sinf(*rotation);
        (*mov).x = vol * cosf(*rotation);
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        (*mov).y = -1 * vol * sinf(*rotation);
        (*mov).x = -1 * vol * cosf(*rotation);
    }
    else if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_A))
    {
        (*mov).y = vol * sin(*rotation + (PI * 0.5));
        (*mov).x = vol * cos(*rotation + (PI * 0.5));
    }
    else if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_D))
    {
        (*mov).y = vol * sinf(*rotation - (PI * 0.5));
        (*mov).x = vol * cosf(*rotation - (PI * 0.5));
    }
    if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT))
    {
        *rotation -= ROTATION_SPEED * GetFrameTime();
        if (*rotation < 0)
        {
            *rotation += 2 * PI;
        }
    }
    if (IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT))
    {
        *rotation += ROTATION_SPEED * GetFrameTime();
        if (*rotation > 2 * PI)
        {
            *rotation -= 2 * PI;
        }
    }
}
