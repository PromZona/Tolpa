#include "renderer.hpp"
#include "raylib.h"

#include "EntityManager.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/TransformComponent.hpp"

Renderer::Renderer() = default;
Renderer::~Renderer() = default;

void Renderer::Render()
{
	auto& entityManager = EntityManager::Instance();
	auto& renders = entityManager.GetComponents<RenderComponent>();
	auto& transforms = entityManager.GetComponents<TransformComponent>();
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawFPS(5, 5);
	for (auto& rendComp : renders)
	{
		auto& transform = transforms[rendComp.TransformComponentIndex];
		DrawCircle((int)transform.Position.x, (int)transform.Position.y, rendComp.Radius, rendComp.Color);
	}
	EndDrawing();
}