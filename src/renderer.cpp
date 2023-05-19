#include "renderer.hpp"
#include "raylib.h"

#include "EntityManager.hpp"
#include "PositionComponent.h"

Renderer::Renderer()
{

}
Renderer::~Renderer()
{

}

void Renderer::Render()
{
    auto& entityManager = EntityManager::Instance();
    auto& positions = entityManager.GetComponents<Position>();
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("test", 0, 0, 14, BLACK);

        for (auto& pos : positions)
        {
            DrawCircle(pos.x, pos.y, 2, RED);
        }
    EndDrawing();
}