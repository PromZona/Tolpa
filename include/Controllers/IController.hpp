#pragma once

#include "ECS/System.hpp"

class IController : public System
{
public:
	virtual ~IController()
	{
	}
	virtual void Tick(float deltaTime) = 0;
};
