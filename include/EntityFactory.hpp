#pragma once

#include <string>
#include <memory>
#include "ECS/ECSTypes.hpp"

class ECS;

class EntityFactory
{
public:
	static EntityId CreateParty();
	static EntityId CreateCity();
	static EntityId CreateLair();
	static EntityId CreateEnemy();
};
