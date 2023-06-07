#include "renderer.hpp"
#include "raylib.h"

#include "ECS/ECS.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/TransformComponent.hpp"

#include "game.hpp"

Renderer::Renderer() = default;
Renderer::~Renderer() = default;

void Renderer::Render()
{
	auto& ecs = Game::Instance().GetECS();
	auto archetypes = ecs.GetRequiredArchetypes(Archetype);

	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawFPS(5, 5);
	for (auto& archetype : archetypes)
	{
		auto& transforms = archetype->GetComponents<TransformComponent>();
		auto& renders = archetype->GetComponents<RenderComponent>();
		std::size_t size = transforms.size();
		for (int i = 0; i < size; i++)
		{
			DrawCircle((int)transforms[i].Position.x, (int)transforms[i].Position.y, renders[i].Radius, renders[i].color);
		}
	}
	EndDrawing();
}