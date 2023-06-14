#pragma once

#include "CommandManager.hpp"
#include "raylib.h"

class CreateCityCommand : public ICommand
{
public:
	CreateCityCommand();
	CreateCityCommand(Vector3 position);
	~CreateCityCommand() override = default;

	void Execute() override;
private:
	Vector3 m_Position;
};
