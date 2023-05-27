#pragma once

#include "IController.hpp"

class UnitGoalController : public IController
{
public:
	UnitGoalController();
	~UnitGoalController() override;
	void Tick(float deltaTime);
};
