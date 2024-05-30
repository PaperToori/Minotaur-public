#include "/opt/homebrew/Cellar/raylib/4.5.0/include/raylib.h"
#include "mylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Initialise variable information
void M_Init(short map[MAP_WIDTH][MAP_HEIGHT],
            short displayedMap[MAP_WIDTH][MAP_HEIGHT],
            Vector2 *pos,
            short *points)
{
    // Initial Values
    (*pos).x = START_POSITION_X * CUBE_WIDTH + BODY_RAD * 1.5;
    (*pos).y = START_POSITION_Y * CUBE_WIDTH + BODY_RAD * 1.5;
    (*points) = 0;

    // Empties map array, and hides all squares
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_HEIGHT; j++)
        {
            map[i][j] = IS_WALL;
            displayedMap[i][j] = 0;
        }
    }
}

void CheckEmptyCells(short map[MAP_WIDTH][MAP_HEIGHT],
                     Locloc position,
                     Collisions *walls)
{
    // Reset
    (*walls).up = false;
    (*walls).down = false;
    (*walls).left = false;
    (*walls).right = false;
    (*walls).deadend = false;

    // Up
    if (1 > (position.y - 2)) // Map border check
    {
        (*walls).up = true;
    }
    else if (IS_NOT_WALL == map[position.x][position.y - 2]) // Path collision check
    {
        (*walls).up = true;
    }
    // Down
    if (MAP_HEIGHT - 2 < position.y + 2) // Map border check
    {
        (*walls).down = true;
    }
    if (IS_NOT_WALL == map[position.x][position.y + 2]) // Path collision check
    {
        (*walls).down = true;
    }
    // Left
    if (1 > position.x - 2) // Map border check
    {
        (*walls).left = true;
    }
    if (IS_NOT_WALL == map[position.x - 2][position.y]) // Path collision check
    {
        (*walls).left = true;
    }
    // Right
    if (MAP_WIDTH - 2 < position.x + 2) // Map border check
    {
        (*walls).right = true;
    }
    if (IS_NOT_WALL == map[position.x + 2][position.y]) // Path collision check
    {
        (*walls).right = true;
    }

    // if every path is blocked
    if ((*walls).up &&
        (*walls).down &&
        (*walls).left &&
        (*walls).right)
    {
        (*walls).deadend = true;
    }

    return;
}

void StepBack(Locloc *positionHistory,
              int *stepsTaken,
              Locloc *position,
              Locloc *ends,
              int *totalEnds,
              bool *end)
{

    // If it is at the end of a possible path, then it will remember the finishing point.
    if (*end)
    {
        *end = false;
        ends[*totalEnds].x = (*position).x;
        ends[*totalEnds].y = (*position).y;
        (*totalEnds)++;
    }

    // takes a step back in the log, and updates position to match
    (*stepsTaken) -= 1;
    (*position).x = positionHistory[(*stepsTaken) - 1].x;
    (*position).y = positionHistory[(*stepsTaken) - 1].y;
}

// Performs the depth-search algorithm updates map
void GenerateMaze(short map[MAP_WIDTH][MAP_HEIGHT],
                  short displayedMap[MAP_WIDTH][MAP_HEIGHT],
                  Vector2 pos)
{

    // Reset map
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_HEIGHT; j++)
        {
            map[i][j] = IS_WALL;
            displayedMap[i][j] = 0;
        }
    }

    srand(time(NULL));

    int randomNumber = 0;

    // Edges, and the count of how many
    Locloc ends[ALL_CELLS_VISITED];
    int totalEnds = 0;
    bool end = true;

    // Position history, and visited cells
    Locloc positionHistory[ALL_CELLS_VISITED];
    int stepsTaken = 0;
    int visitedCells = 0;
    bool moved = false;

    // Current position and check unvisited cells
    Collisions walls;
    Locloc position;
    position.x = (int)pos.x / CUBE_WIDTH;
    position.y = (int)pos.y / CUBE_WIDTH;

    // Set player position to be the first visited cells
    map[position.x][position.y] = IS_NOT_WALL;
    positionHistory[stepsTaken].x = position.x;
    positionHistory[stepsTaken].y = position.y;
    stepsTaken++;
    visitedCells++;

    while (ALL_CELLS_VISITED > visitedCells)
    {
        /* Values 0 - 3 are equivalent to:
            0 = up
            1 = right
            2 = down
            3 = left
            (like a clock) */
        randomNumber = rand() % 4;
        moved = false;

        CheckEmptyCells(map,
                        position,
                        &walls);

        if (walls.deadend)
        {
            StepBack(positionHistory,
                     &stepsTaken,
                     &position,
                     ends,
                     &totalEnds,
                     &end);
            continue;
        }

        switch (randomNumber)
        {
        case 0:
        {
            if (walls.up)
            {
                randomNumber = 1;
                break;
            }

            position.y -= 2;
            map[position.x][position.y + 1] = IS_NOT_WALL;
            map[position.x][position.y] = IS_NOT_WALL;

            end = true;
            moved = true;

            break;
        }
        case 1:
        {
            if (walls.right)
            {
                randomNumber = 2;
                break;
            }

            position.x += 2;
            map[position.x - 1][position.y] = IS_NOT_WALL;
            map[position.x][position.y] = IS_NOT_WALL;

            end = true;
            moved = true;

            break;
        }
        case 2:
        {
            if (walls.down)
            {
                randomNumber = 3;
                break;
            }

            position.y += 2;
            map[position.x][position.y - 1] = IS_NOT_WALL;
            map[position.x][position.y] = IS_NOT_WALL;

            end = true;
            moved = true;

            break;
        }
        case 3:
        {
            if (walls.left)
            {
                randomNumber = 0;
                break;
            }

            position.x -= 2;
            map[position.x + 1][position.y] = IS_NOT_WALL;
            map[position.x][position.y] = IS_NOT_WALL;

            end = true;
            moved = true;

            break;
        }
        }

        if (moved)
        {
            positionHistory[stepsTaken].x = position.x;
            positionHistory[stepsTaken].y = position.y;
            stepsTaken++;
            visitedCells++;
        }
    }
    randomNumber = rand() % totalEnds;
    map[ends[randomNumber].x][ends[randomNumber].y] = IS_GOAL;
}