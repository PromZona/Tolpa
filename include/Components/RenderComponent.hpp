#pragma once

#include "raylib.h"

struct RenderComponent
{
	Color Color;
	float Radius;

	int TransformComponentIndex;
};