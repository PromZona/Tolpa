#pragma once

#include "CommandManager.hpp"
#include "raylib.h"

class CreateOrcCommand : public ICommand
{
public:
	CreateOrcCommand();
	~CreateOrcCommand() override = default;

	void Execute() override;
};
