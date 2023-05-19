#include "game.hpp"

#include <string>
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "PositionComponent.h"

Game::Game() : m_entityManager(EntityManager::Instance()), m_renderer()
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
    auto& entity = m_entityManager.CreateEntity(std::string("Player"));
    auto& pos = entity.AddComponent<Position>();
    pos.x = 5;
    pos.y = 5;

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
    auto& positions = m_entityManager.GetComponents<Position>();

    float deltaTime = GetFrameTime();
    for (auto& pos : positions)
    {
        pos.x += 1;
    }
}

void Game::Render()
{
    m_renderer.Render();
}
