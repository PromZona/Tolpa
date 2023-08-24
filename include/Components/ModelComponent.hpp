#pragma once

#include "raylib.h"
#include <string>

// Forward declaration
enum class ModelType;

struct ModelComponent
{
    ModelType model_id;
    float scale;
};