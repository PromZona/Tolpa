#pragma once

#include "raylib.h"

struct TransformComponent
{
	Vector3 Position;
	Vector3 RotationAxis = {0.0f, 0.0f, 0.0f};
	float RotationAngle = 0.0f;
	int RotationSwitchFrameDelay;
	float TargetAngle;
	float AngleGrowth;
};
