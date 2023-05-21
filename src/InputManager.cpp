#include "InputManager.hpp"

#include <raylib.h>
#include "EntityManager.hpp"
#include "Entity.hpp"

#include "Components/MovementComponent.hpp"

InputManager::InputManager() : m_entityManager(EntityManager::Instance()) {}
InputManager::~InputManager() = default;

void InputManager::Update(){
    int keyPressed = 0;
    while ((keyPressed = GetKeyPressed()) != 0)
    {
        if (keyPressed == KEY_W)
        {
            auto& newEntity = m_entityManager.CreateEntity("Object");
            auto& component = newEntity.AddComponent<MovementComponent>();
            component.Position = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
            component.Speed = 50.0f;
            component.Direction = {((float)GetRandomValue(-100, 100) / 100.0f), ((float)GetRandomValue(-100, 100) / 100.0f)};
        }
    }
}