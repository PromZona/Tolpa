#pragma once

#include "raylib.h"

struct TransformComponent
{
	Vector3 Position;
	Vector3 RotationAxis;
	float rotationAngle;
};
