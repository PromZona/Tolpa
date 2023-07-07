#pragma once

#include "raylib.h"

struct RotationComponent
{
	Vector3 RotationAxis;
	float CurrentAngle;
	float TargetAngle;
	float AngleGrowth;
    int FramesToRotate;
};