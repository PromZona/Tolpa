#include "renderer.hpp"
#include "raylib.h"

#include "EntityManager.hpp"
#include "Components/MovementComponent.hpp"

Renderer::Renderer()
{

}
Renderer::~Renderer()
{

}

void Renderer::Render()
{
    auto& entityManager = EntityManager::Instance();
    auto& movements = entityManager.GetComponents<MovementComponent>();
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawFPS(5, 5);
        for (auto& mov : movements)
        {
            DrawCircle(mov.Position.x, mov.Position.y, 20, RED);
        }
    EndDrawing();
}