#pragma once

#include "CommandManager.hpp"
#include "raylib.h"

class CreateHumanCommand : public ICommand
{
public:
	CreateHumanCommand();
	CreateHumanCommand(Vector3 position);
	~CreateHumanCommand() override = default;

	void Execute() override;
private:
	Vector3 m_Position;
};
