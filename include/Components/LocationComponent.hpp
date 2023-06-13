#pragma once

#include <string>

// Needed only for renderer to recognize entity as location
// (but can be actually useful later?)
struct LocationComponent
{
    std::string name;
};