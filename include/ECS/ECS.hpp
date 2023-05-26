#pragma once

#include <unordered_map>
#include "Archetype.hpp"
#include "ECSTypes.hpp"

class ECS
{
public:
	ECS();
	~ECS();

	EntityId CreateEntity();
	void DeleteEntity(const EntityId& entityId);

	template<class T>
	void AddComponentToEntity(const EntityId& entityId, T component)
	{

	}

	template<class T>
	T& GetComponent(const EntityId& entityId)
	{
		return T();
	}

	template<class T>
	void DeleteComponentFromEntity(const EntityId& entityId)
	{

	}

private:
	EntityId m_availableEntityId = 0;
	std::unordered_map<EntityId, Archetype> m_entityToArchetype;
};
