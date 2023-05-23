#include "InputManager.hpp"

#include <raylib.h>
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "EntityFactory.hpp"

#include "Components/TransformComponent.hpp"

InputManager::InputManager() : m_entityManager(EntityManager::Instance())
{
}
InputManager::~InputManager() = default;

void InputManager::Update()
{
	int keyPressed = 0;
	while ((keyPressed = GetKeyPressed()) != 0)
	{
		if (keyPressed == KEY_W)
		{
			auto mousePos = GetMousePosition();
			auto& unit = EntityFactory::CreateParty();
			unit.GetComponent<TransformComponent>().Position = mousePos;
			continue;
		}
    
		if (keyPressed == KEY_E)
		{
			auto mousePos = GetMousePosition();
			auto& city = EntityFactory::CreateCity();
			city.GetComponent<TransformComponent>().Position = mousePos;
			continue;
		}
		if (keyPressed == KEY_R)
		{
			auto mousePos = GetMousePosition();
			auto& lair = EntityFactory::CreateLair();
			lair.GetComponent<TransformComponent>().Position = mousePos;
			continue;
		}
	}
}