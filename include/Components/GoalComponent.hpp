#pragma once

#include "raylib.h"
#include <vector>

struct GoalComponent
{
	Vector3 GoalPosition;
	std::vector<Vector3> PathToGoal;
	bool IsActive;
	int steps;
};
