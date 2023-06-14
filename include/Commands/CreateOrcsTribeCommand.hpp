#pragma once

#include "CommandManager.hpp"
#include "raylib.h"

class CreateOrcsTribeCommand : public ICommand
{
public:
	CreateOrcsTribeCommand();
	CreateOrcsTribeCommand(Vector3 position);
	~CreateOrcsTribeCommand() override = default;

	void Execute() override;
private:
	Vector3 m_Position;
};
