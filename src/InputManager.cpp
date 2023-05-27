#include "InputManager.hpp"

#include <raylib.h>
#include "EntityFactory.hpp"
#include "game.hpp"

#include "Components/TransformComponent.hpp"

InputManager::InputManager()
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
			auto unit = EntityFactory::CreateParty();
			auto& ecs = Game::Instance().GetECS();
			ecs.GetComponent<TransformComponent>(unit)->Position = mousePos;
			continue;
		}
    
		if (keyPressed == KEY_E)
		{
			auto mousePos = GetMousePosition();
			auto city = EntityFactory::CreateCity();
			auto& ecs = Game::Instance().GetECS();
			auto transform= ecs.GetComponent<TransformComponent>(city);
			transform->Position = mousePos;
			continue;
		}
		if (keyPressed == KEY_R)
		{
			auto mousePos = GetMousePosition();
			auto lair = EntityFactory::CreateLair();
			auto& ecs = Game::Instance().GetECS();
			ecs.GetComponent<TransformComponent>(lair)->Position = mousePos;
			continue;
		}

		if (keyPressed == KEY_Z)
		{
			Game::Instance().GetECS().PrintInfo();
		}
	}
}