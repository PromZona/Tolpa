#pragma once

#include <vector>
#include "ECS/ECSTypes.hpp"

struct GameState
{
	std::vector<EntityId> cities;
	std::vector<EntityId> parties;
	std::vector<EntityId> lairs;
	std::vector<EntityId> enemies;
};
