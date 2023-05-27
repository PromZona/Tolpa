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
	void PrintInfo();

	template<class T>
	void AddComponentToEntity(const EntityId& entityId, T component)
	{
		if (m_entityToArchetype.find(entityId) == m_entityToArchetype.end())
		{
			TraceLog(LOG_WARNING, "Failed to add Component to unregistered Entity[%d]", entityId);
			throw std::out_of_range("Failed to find entity");
		}
		auto entityCurrentArchetype = m_entityToArchetype[entityId];

		auto archetypeId = entityCurrentArchetype->GetId();
		int componentBit = GetOrCreateComponentBit(std::type_index(typeid(T)));
		archetypeId.set(componentBit);
		auto newArchetype = GetOrCreateArchetype(archetypeId);
		newArchetype->AddComponent(entityId, component, componentBit);
		newArchetype->MoveComponentsFromOldArchetype(entityCurrentArchetype, entityId);
		m_entityToArchetype[entityId] = newArchetype;
	}

	template<class T>
	T& GetComponent(const EntityId& entityId)
	{
		if (m_entityToArchetype.find(entityId) == m_entityToArchetype.end())
		{
			TraceLog(LOG_WARNING, "Failed to return Component of unregistered Entity[%d]", entityId);
			throw std::out_of_range("Failed to find entity");
		}
		return m_entityToArchetype[entityId]->GetComponent<T>(entityId);
	}

	template<class T>
	void DeleteComponentFromEntity(const EntityId& entityId)
	{

	}

private:
	EntityId m_availableEntityId = 0;
	std::unordered_map<ArchetypeId, std::shared_ptr<Archetype>> m_archetypes;
	std::unordered_map<EntityId, std::shared_ptr<Archetype>> m_entityToArchetype;
	std::unordered_map<std::type_index, int> m_componentTypeToBit;

	std::shared_ptr<Archetype> GetOrCreateArchetype(ArchetypeId archetypeId);
	int GetOrCreateComponentBit(std::type_index componentType);
};
