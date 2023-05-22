#include "renderer.hpp"
#include "raylib.h"

#include "EntityManager.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/RenderComponent.hpp"

Renderer::Renderer()
{

}
Renderer::~Renderer()
{

}

void Renderer::Render()
{
    auto& entityManager = EntityManager::Instance();
    auto& renders = entityManager.GetComponents<RenderComponent>();
    auto& movements = entityManager.GetComponents<MovementComponent>();
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawFPS(5, 5);
        for (auto& rendComp : renders)
        {
            auto& moveComp = movements[rendComp.TransformComponentIndex];
            DrawCircle((int)moveComp.Position.x, (int)moveComp.Position.y, rendComp.Radius, rendComp.Color);
        }
    EndDrawing();
}