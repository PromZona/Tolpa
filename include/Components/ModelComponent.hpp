#pragma once

#include "raylib.h"
#include <string>

struct ModelComponent
{
    std::shared_ptr<Model> model;
    float scale;
};