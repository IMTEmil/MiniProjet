/*******************************************************************************************
 *
 *   raylib - classic game: snake
 *
 *   Sample game developed by Ian Eito, Albert Martos and Ramon Santamaria
 *
 *   This game has been created using raylib v1.3 (www.raylib.com)
 *   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
 *
 *   Copyright (c) 2015 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include <string.h>
#include "projet.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define SNAKE_LENGTH 256
#define SQUARE_SIZE 31

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct Snake
{
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Color color;
} Snake;

typedef struct Food
{
    Vector2 position;
    Vector2 size;
    bool active;
    Color color;
} Food;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;

static int framesCounter = 0;
static bool gameOver = false;
static bool pause = false;

static Food fruit = {0};
static Snake snake[SNAKE_LENGTH] = {0};
static Vector2 snakePosition[SNAKE_LENGTH] = {0};
static bool allowMove = false;
static Vector2 offset = {0};
static int counterTail = 0;

static bool isFruit = true;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);        // Initialize game
static void UpdateGame(void);      // Update game (one frame)
static void DrawGame(void);        // Draw game (one frame)
static void UnloadGame(void);      // Unload game
static void UpdateDrawFrame(void); // Update and Draw (one frame)

static GAMESTATE GameState = GS_MENU;

static GAME_SENEQUE GameSeneque = { 0 };

static Liste snares = NULL;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "classic game: snake ?");

    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
    }
#endif
    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGame(); // Unload loaded data (textures, sounds, models...)

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(void)
{
    framesCounter = 0;
    gameOver = false;
    pause = false;

    counterTail = 1;
    allowMove = false;

    offset.x = screenWidth % SQUARE_SIZE;
    offset.y = screenHeight % SQUARE_SIZE;

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        snake[i].position = (Vector2){offset.x / 2, offset.y / 2};
        snake[i].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
        snake[i].speed = (Vector2){SQUARE_SIZE, 0};

        if (i == 0)
            snake[i].color = DARKBLUE;
        else
            snake[i].color = BLUE;
    }

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        snakePosition[i] = (Vector2){0.0f, 0.0f};
    }

    fruit.size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    fruit.color = SKYBLUE;
    fruit.active = false;

    InitProjetAddOn(&GameSeneque, &snares);
}

// Update game (one frame)
void UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed('P'))
            pause = !pause;

        if (!pause)
        {
            // Player control
            if (IsKeyPressed(KEY_RIGHT) && (snake[0].speed.x == 0) && allowMove)
            {
                snake[0].speed = (Vector2){SQUARE_SIZE, 0};
                allowMove = false;
            }
            if (IsKeyPressed(KEY_LEFT) && (snake[0].speed.x == 0) && allowMove)
            {
                snake[0].speed = (Vector2){-SQUARE_SIZE, 0};
                allowMove = false;
            }
            if (IsKeyPressed(KEY_UP) && (snake[0].speed.y == 0) && allowMove)
            {
                snake[0].speed = (Vector2){0, -SQUARE_SIZE};
                allowMove = false;
            }
            if (IsKeyPressed(KEY_DOWN) && (snake[0].speed.y == 0) && allowMove)
            {
                snake[0].speed = (Vector2){0, SQUARE_SIZE};
                allowMove = false;
            }

            // Snake movement
            for (int i = 0; i < counterTail; i++) snakePosition[i] = snake[i].position;

            if ((framesCounter % 5) == 0)
            {
                for (int i = 0; i < counterTail; i++)
                {
                    if (i == 0)
                    {
                        snake[0].position.x += snake[0].speed.x;
                        snake[0].position.y += snake[0].speed.y;
                        allowMove = true;
                    }
                    else
                        snake[i].position = snakePosition[i - 1];
                }
            }

            // Wall behaviour
            if (((snake[0].position.x) > (screenWidth - offset.x)) ||
                ((snake[0].position.y) > (screenHeight - offset.y)) ||
                (snake[0].position.x < 0) || (snake[0].position.y < 0))
            {
                if (GameState != GS_SNARE) gameOver = true;
                else {
                    if ((snake[0].position.x) > (screenWidth - offset.x)) snake[0].position.x = offset.x/2;
                    else if ((snake[0].position.y) > (screenHeight - offset.y)) snake[0].position.y = offset.y/2;
                    else if (snake[0].position.x < 0) snake[0].position.x = screenWidth - offset.x / 2 - SQUARE_SIZE;
                    else if (snake[0].position.y < 0) snake[0].position.y = screenHeight - offset.y / 2 - SQUARE_SIZE;
                }
            }

            // Collision with yourself
            for (int i = 1; i < counterTail; i++)
            {
                if ((snake[0].position.x == snake[i].position.x) && (snake[0].position.y == snake[i].position.y))
                    gameOver = true;
            }

            // Fruit position calculation
            if (isFruit && !fruit.active)
            {
                fruit.active = true;
                fruit.position = (Vector2){GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};

                for (int i = 0; i < counterTail; i++)
                {
                    while (((fruit.position.x == snake[i].position.x) && (fruit.position.y == snake[i].position.y))
                    || (GameState != GS_SNARE ? false : SnareAlreadyAtPosition(snares, fruit.position)))
                    {
                        fruit.position = (Vector2){GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
                        i = 0;
                    }
                }

            }
            
            // Collision
            if ((snake[0].position.x < (fruit.position.x + fruit.size.x) && (snake[0].position.x + snake[0].size.x) > fruit.position.x) &&
                (snake[0].position.y < (fruit.position.y + fruit.size.y) && (snake[0].position.y + snake[0].size.y) > fruit.position.y))
            {
                snake[counterTail].position = snakePosition[counterTail - 1];
                counterTail += 1;
                fruit.active = false;

                IfCollisionSendCitation(&(GameSeneque), framesCounter);
            }
            
            if (SnareCollision(snares, snake[0].position))
            {
                gameOver = true;
            }

            if (GameState == GS_SNARE)
            {
                UpdateSnares(&snares, 1, 50, fruit.position);
            }

            displayCitation(&GameSeneque, 1, framesCounter);
            framesCounter++;
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
        }

        if (IsKeyPressed('I'))
        {
            gameOver = true;
            GameState = GS_MENU;
        }
    }
}

// Draw Menu (if GS_MENU)
void DrawMenu(void)
{
    ClearBackground(RAYWHITE);

    // Draw text
    DrawText("Choose your game !", (GetScreenWidth() - MeasureText("Choose your game !", 50)) / 2, 40, 50, LIGHTGRAY);
    DrawText("\'1\'. SNAKE", 50, 140, MENU_CHOICE_FONT_SIZE, LIGHTGRAY);
    DrawText("\'2\'. SENEQUE", 400, 140, MENU_CHOICE_FONT_SIZE, LIGHTGRAY);
    DrawText("\'3\'. SNARE", 50, 300, MENU_CHOICE_FONT_SIZE, LIGHTGRAY);
    DrawText("\'4\'. SNACK", 400, 300, MENU_CHOICE_FONT_SIZE, LIGHTGRAY);

    // Wait for user input
    if (IsKeyPressed('1') == true)
        GameState = GS_NORMAL;

    if (IsKeyPressed('2') == true)
        GameState = GS_SENEQUE;

    if (IsKeyPressed('3') == true)
        GameState = GS_SNARE;

    if (IsKeyPressed('4') == true)
        GameState = GS_SNACK;
}

void DrawSeneque(void)
{
    if (GameState == GS_SENEQUE)
    {
        DrawTexture(GameSeneque.SenequeHeadImage, snake[0].position.x, snake[0].position.y, WHITE);

        if (GameSeneque.isCitation == true)
        {
            DrawText(CitationsSeneque[GameSeneque.indexCitation], screenWidth / 2 - MeasureText(CitationsSeneque[GameSeneque.indexCitation], 21) / 2, screenHeight / 2 - 21, 23, DARKBLUE);
        }
    }
}

// Draw game (one frame)
void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (GameState == GS_MENU)
    {
        DrawMenu();
    }
    else if (!gameOver && (GameState != GS_MENU))
    {
        // Draw grid lines : always
        for (int i = 0; i < screenWidth / SQUARE_SIZE + 1; i++)
        {
            DrawLineV((Vector2){SQUARE_SIZE * i + offset.x / 2, offset.y / 2}, (Vector2){SQUARE_SIZE * i + offset.x / 2, screenHeight - offset.y / 2}, LIGHTGRAY);
        }

        for (int i = 0; i < screenHeight / SQUARE_SIZE + 1; i++)
        {
            DrawLineV((Vector2){offset.x / 2, SQUARE_SIZE * i + offset.y / 2}, (Vector2){screenWidth - offset.x / 2, SQUARE_SIZE * i + offset.y / 2}, LIGHTGRAY);
        }

        // Draw snake : always
        for (int i = 0; i < counterTail; i++) DrawRectangleV(snake[i].position, snake[i].size, snake[i].color);

       
        // Draw fruit to pick
        DrawRectangleV(fruit.position, fruit.size, fruit.color);

        if (GameState == GS_SNARE)
        {
            DrawSnares(snares);
        }

        DrawSeneque();

        if (pause)
            DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
    }
    else
    {
        if (GameState == GS_SENEQUE) DrawText("C'EST PAS GRAVE SENEQU'UN JEU !", GetScreenWidth() / 2 - MeasureText("C'EST PAS GRAVE SENEQU'UN JEU !", 20) / 2, GetScreenHeight() / 2 - 100, 20, GRAY);
        if (GameState == GS_SNARE) DrawText("SNARE'IEN, RECOMMENCE !", GetScreenWidth() / 2 - MeasureText("SNARE'IEN, RECOMMENCE !", 20) / 2, GetScreenHeight() / 2 - 100, 20, GRAY);
        DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
        DrawText("OR PRESS [I] TO GO BACK TO MENU", GetScreenWidth() / 2 - MeasureText("OR PRESS [I] TO GO BACK TO MENU", 20) / 2, GetScreenHeight() / 2, 20, GRAY);
    }

    EndDrawing();
}

// Unload game variables
void UnloadGame(void)
{
    CloseProjetAddOn(&GameSeneque, &snares);
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}
