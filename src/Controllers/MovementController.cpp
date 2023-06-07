#include "Controllers/MovementController.hpp"

#include <vector>
#include "Components/MovementComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "game.hpp"

#define RAYMATH_IMPLEMENTATION
#include "raymath.h"

MovementController::MovementController()
{
}
MovementController::~MovementController() = default;

void MovementController::Tick(float deltaTime)
{
	// TODO: Add support to new ECS
	auto& ecs = Game::Instance().GetECS();

	auto archetypes = ecs.GetRequiredArchetypes(Archetype);

	for (auto archetype : archetypes)
	{
		auto& moves = archetype->GetComponents<MovementComponent>();
		auto& trans = archetype->GetComponents<TransformComponent>();
		std::size_t size = moves.size();
		for (int i = 0; i < size; i++)
		{
			auto& movement = moves[i];
			auto& transform = trans[i];
			Vector3 result = Vector3Add(transform.Position, Vector3Scale(movement.Direction, movement.Speed * deltaTime));
			transform.Position = result;
		}
	}
}
