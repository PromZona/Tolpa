#pragma once

#include <string>
#include <memory>
#include "ECS/ECSTypes.hpp"

class ECS;

class EntityFactory
{
public:
	static EntityId CreateParty(const std::string model_path);
	static EntityId CreateCity(const std::string model_path);
	static EntityId CreateLair(const std::string model_path);
	static EntityId CreateEnemy(const std::string model_path);
};
