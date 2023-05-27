#pragma once

#include "ECS/System.hpp"

class Renderer : public System
{
public:
	Renderer();
	~Renderer();

	void Render();
};