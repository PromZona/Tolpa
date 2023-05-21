#include "Controllers/MovementController.hpp"

#include "EntityManager.hpp"
#include "Components/MovementComponent.hpp"

MovementController::MovementController() : m_entityManager(EntityManager::Instance()) {}
MovementController::~MovementController() = default;

void MovementController::Tick(float deltaTime) {
    auto& movementComponents = m_entityManager.GetComponents<MovementComponent>();

    for (auto& moveComp : movementComponents)
    {
        moveComp.Position = Vector2Add(moveComp.Position, Vector2Scale(moveComp.Direction, moveComp.Speed * deltaTime));
    }
}
