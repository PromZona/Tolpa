#pragma once

class IController
{
public:
	virtual ~IController()
	{
	}
	virtual void Tick(float deltaTime) = 0;
};
