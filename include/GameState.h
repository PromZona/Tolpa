#pragma once

#include <vector>
#include "ECS/ECSTypes.hpp"

struct GameState
{
	std::vector<EntityId> HumanCities;
	std::vector<EntityId> Humans;
	std::vector<EntityId> OrcTribes;
	std::vector<EntityId> Orcs;
};
