#include "game.hpp"

#include <string>
#include "EntityManager.hpp"
#include "Entity.hpp"

struct Position
{
    int x, y;
};

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
    auto& manager = EntityManager::Instance();
    auto& entity = manager.CreateEntity(std::string("Player"));
    auto& pos = entity.AddComponent<Position>();
    pos.x = 20;
    pos.y = 30;

    while (!WindowShouldClose()) {
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
