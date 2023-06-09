#pragma once

#include "CommandManager.hpp"
#include "raylib.h"

class CreateOrcsTribeCommand : public ICommand
{
public:
	CreateOrcsTribeCommand();
	CreateOrcsTribeCommand(Vector2 position);
	~CreateOrcsTribeCommand() override = default;

	void Execute() override;
private:
	Vector2 m_Position;
};
