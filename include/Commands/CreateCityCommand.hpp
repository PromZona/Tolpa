#pragma once

#include "CommandManager.hpp"
#include "raylib.h"

class CreateCityCommand : public ICommand
{
public:
	CreateCityCommand();
	CreateCityCommand(Vector2 position);
	~CreateCityCommand() override = default;

	void Execute() override;
private:
	Vector2 m_Position;
};
