#include "InputManager.hpp"

#include <raylib.h>
#include "EntityManager.hpp"
#include "Entity.hpp"

#include "Components/MovementComponent.hpp"
#include "Components/RenderComponent.hpp"

InputManager::InputManager() : m_entityManager(EntityManager::Instance()) {}
InputManager::~InputManager() = default;

void InputManager::Update(){
    int keyPressed = 0;
    while ((keyPressed = GetKeyPressed()) != 0)
    {
        if (keyPressed == KEY_W)
        {
            auto& newEntity = m_entityManager.CreateEntity("Object");
            int moveCompId;
            auto& moveComp = newEntity.AddComponent<MovementComponent>(moveCompId);
            moveComp.Position = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
            moveComp.Speed = 50.0f;
            moveComp.Direction = Vector2Normalize({((float)GetRandomValue(-100, 100)), ((float)GetRandomValue(-100, 100))});

            auto& rendComp = newEntity.AddComponent<RenderComponent>();
            rendComp.Color = {static_cast<unsigned char>(GetRandomValue(0, 255)),
                              static_cast<unsigned char>(GetRandomValue(0, 255)),
                              static_cast<unsigned char>(GetRandomValue(0, 255)),
                              255};
            rendComp.Radius = 8;
            rendComp.TransformComponentIndex = moveCompId;
        }
    }
}