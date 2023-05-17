#include "game.hpp"

Game::Game()
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Tolpa");
    SetTargetFPS(60);
}

Game::~Game()
{
    CloseWindow();
}

void Game::Start()
{
    while (!WindowShouldClose())
    {
        Input();
        Update();
        Render();
    }
}

void Game::Input()
{

}

void Game::Update()
{
    
}

void Game::Render()
{
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("test", 0, 0, 14, BLACK);
    EndDrawing();
}
