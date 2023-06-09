#pragma once

#include "CommandManager.hpp"
#include "raylib.h"

class CreateHumanCommand : public ICommand
{
public:
	CreateHumanCommand();
	CreateHumanCommand(Vector2 position);
	~CreateHumanCommand() override = default;

	void Execute() override;
private:
	Vector2 m_Position;
};
