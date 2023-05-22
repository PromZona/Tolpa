#pragma once

#include "raylib.h"

struct MovementComponent
{
    int PositionComponentIndex;
    Vector2 Direction;
    float Speed;
};