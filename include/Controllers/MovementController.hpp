#pragma once

#include "IController.hpp"

class EntityManager;

class MovementController : public IController
{
public:
	MovementController();
	~MovementController() override;

	void Tick(float deltaTime);
};
