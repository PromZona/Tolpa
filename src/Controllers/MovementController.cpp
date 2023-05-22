#include "Controllers/MovementController.hpp"

#include "EntityManager.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/TransformComponent.hpp"

#define RAYMATH_IMPLEMENTATION
#include "raymath.h"

MovementController::MovementController() : m_entityManager(EntityManager::Instance()) {}
MovementController::~MovementController() = default;

void MovementController::Tick(float deltaTime) {
    auto& movementComponents = m_entityManager.GetComponents<MovementComponent>();
    auto& transformComponents = m_entityManager.GetComponents<TransformComponent>();

    for (auto& moveComp : movementComponents)
    {
        auto& transform = transformComponents[moveComp.PositionComponentIndex];
        Vector2 result =  Vector2Add(transform.Position, Vector2Scale(moveComp.Direction, moveComp.Speed * deltaTime));
        transform.Position = result;
    }
}
