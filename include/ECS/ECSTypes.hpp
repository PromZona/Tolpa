#pragma once

#include <bitset>

using EntityId = unsigned int;

const int MAX_COMPONENTS = 64;

using ArchetypeId = std::bitset<MAX_COMPONENTS>;
