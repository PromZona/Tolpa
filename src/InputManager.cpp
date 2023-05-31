#include "InputManager.hpp"

#include <raylib.h>
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "EntityFactory.hpp"

#include "Components/TransformComponent.hpp"

InputManager::InputManager() : m_entityManager(EntityManager::Instance()) {}
InputManager::~InputManager() = default;

void InputManager::Update(){
    int keyPressed = 0;
    while ((keyPressed = GetKeyPressed()) != 0)
    {
        if (keyPressed == KEY_O)
        {
            auto mousePos = GetMousePosition();
            auto& unit = EntityFactory::CreateUnit();
            //unit.GetComponent<TransformComponent>().Position = mousePos;
            continue;
        }

        if (keyPressed == KEY_P)
        {
            auto mousePos = GetMousePosition();
            auto& city = EntityFactory::CreateCity();
            //city.GetComponent<TransformComponent>().Position = mousePos;
            continue;
        }
    }
}