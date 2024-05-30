#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 1200
#define CUBE_WIDTH 25
#define BODY_RAD 6
#define RAYCOUNT 1200
#define VIEW_GIRTH 30
#define ROTATION_SPEED 3

#define TEXTURE_SIZE 25
#define TEXTURE_HEIGHT 12

// Map sizes need to be uneven numbers
#define MAP_WIDTH 15
#define MAP_HEIGHT 15
#define MAP_GRID_SQUARE_SIZE 10
#define MAP_DISPLAY_OFFSET_X ((SCREEN_WIDTH/2) - ((MAP_GRID_SQUARE_SIZE * MAP_WIDTH) / 2))
#define MAP_DISPLAY_OFFSET_Y ((SCREEN_HEIGHT/2) - ((MAP_GRID_SQUARE_SIZE * MAP_HEIGHT) / 2))

#define ALL_CELLS_VISITED (((MAP_WIDTH - 1) / 2) * ((MAP_HEIGHT - 1) / 2))
#define IS_WALL 1
#define IS_NOT_WALL 0
#define IS_GOAL 2
#define START_POSITION_X 1
#define START_POSITION_Y 1

#define LEVEL_TIMER 300
#define SCORE_DISPLAY_TIMER 4

// British ftw
#define Colour Color

float visionCone = (PI * 0.6);
float stepSize = 0.01;
float viewGirth = 6;

enum GameState
{
    menu,
    game,
    deathscreen
};
enum ViewState
{
    bird,
    head
};
enum Status
{
    alive,
    dead
};

struct Laser
{
    float length;
    float angle;
    Vector2 edge;
    Vector2 start;
    short spotted;
    short x;
    short y;
};

struct Locloc
{
    int x;
    int y;
};

struct Collisions
{
    bool up;
    bool down;
    bool left;
    bool right;
    bool deadend;
};