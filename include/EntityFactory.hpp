#pragma once

#include <string>
#include "ECS/ECSTypes.hpp"

class ECS;
struct Vector2;

class EntityFactory
{
public:
	static EntityId CreateHuman(Vector2 position);
	static EntityId CreateCity(Vector2 position);
	static EntityId CreateOrcsTribe(Vector2 position);
	static EntityId CreateOrc(Vector2 position);
};
