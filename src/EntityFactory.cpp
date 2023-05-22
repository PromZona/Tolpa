#include "EntityFactory.hpp"

#include "EntityManager.hpp"
#include "Entity.hpp"

#include "Components/TransformComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/RenderComponent.hpp"


Entity& EntityFactory::CreateUnit() {
    auto& entityManager = EntityManager::Instance();
    auto& unit = entityManager.CreateEntity("Unit");

    int transformIndex;
    auto& transformComponent = unit.AddComponent<TransformComponent>(transformIndex);
    transformComponent.Position = {0, 0};

    auto& renderComponent = unit.AddComponent<RenderComponent>();
    renderComponent.TransformComponentIndex = transformIndex;
    renderComponent.Radius = 4;
    renderComponent.Color = GREEN;

    auto& movementComponent = unit.AddComponent<MovementComponent>();
    movementComponent.PositionComponentIndex = transformIndex;
    movementComponent.Direction = {0, 0};
    movementComponent.Speed = 20.0f;

    return unit;
}

Entity& EntityFactory::CreateCity() {
    auto& entityManager = EntityManager::Instance();
    auto& city = entityManager.CreateEntity("City");

    int transformIndex;
    auto& transformComponent = city.AddComponent<TransformComponent>(transformIndex);
    transformComponent.Position = {0, 0};

    auto& renderComponent = city.AddComponent<RenderComponent>();
    renderComponent.TransformComponentIndex = transformIndex;
    renderComponent.Radius = 8;
    renderComponent.Color = RED;

    return city;
}
