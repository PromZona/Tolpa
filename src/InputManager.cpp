#include "InputManager.hpp"

#include <raylib.h>
#include "EntityFactory.hpp"
#include "game.hpp"

#include "Components/TransformComponent.hpp"
#include "Commands/CreateCityCommand.hpp"
#include "Commands/CreateHumanCommand.hpp"
#include "Commands/CreateOrcsTribeCommand.hpp"
#include "Commands/CreateOrcCommand.hpp"

InputManager::InputManager()
{
}
InputManager::~InputManager() = default;

void InputManager::Update()
{
	int keyPressed = 0;
	while ((keyPressed = GetKeyPressed()) != 0)
	{
		if (keyPressed == KEY_Q)
		{
			auto command = std::make_unique<CreateOrcCommand>();
			Game::Instance().GetCommandManager().AddCommand(std::move(command));
			continue;
		}

		if (keyPressed == KEY_W)
		{
			auto mousePos = GetMousePosition();
			auto command = std::make_unique<CreateHumanCommand>(mousePos);
			Game::Instance().GetCommandManager().AddCommand(std::move(command));
			continue;
		}
    
		if (keyPressed == KEY_E)
		{
			auto mousePos = GetMousePosition();
			auto command = std::make_unique<CreateCityCommand>(mousePos);
			Game::Instance().GetCommandManager().AddCommand(std::move(command));
			continue;
		}

		if (keyPressed == KEY_R)
		{
			auto mousePos = GetMousePosition();
			auto command = std::make_unique<CreateOrcsTribeCommand>(mousePos);
			Game::Instance().GetCommandManager().AddCommand(std::move(command));
			continue;
		}

		if (keyPressed == KEY_Z)
		{
			Game::Instance().GetECS().PrintInfo();
		}

		if (keyPressed == KEY_GRAVE)
		{
			Game::Instance().GetGUI().SwitchConsoleWindow();
		}

		if (keyPressed == KEY_ONE)
		{
			Game::Instance().GetGUI().SwitchECSWindow();
		}
	}
}