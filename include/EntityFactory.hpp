#pragma once

#include <string>
#include <memory>
#include "ECS/ECSTypes.hpp"

class ECS;
struct Vector3;

class EntityFactory
{
public:
	static EntityId CreateHuman(Vector3 position);
	static EntityId CreateCity(Vector3 position);
	static EntityId CreateOrcsTribe(Vector3 position);
	static EntityId CreateOrc(Vector3 position);
};
