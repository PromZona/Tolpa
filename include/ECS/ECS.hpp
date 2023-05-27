#pragma once

#include <unordered_map>
#include "Archetype.hpp"
#include "ECSTypes.hpp"
#include "System.hpp"

class ECS
{
public:
	ECS();
	~ECS();

	EntityId CreateEntity();
	void DeleteEntity(const EntityId& entityId);
	void PrintInfo();
	void CheckEntity(const EntityId& entity);

	template<class T>
	void AddComponent(const EntityId& entityId, T component)
	{
		CheckEntity(entityId);

		auto entityCurrentArchetype = m_entityToArchetype[entityId];
		auto archetypeId = entityCurrentArchetype->GetId();
		int componentBit = GetOrCreateComponentBit(std::type_index(typeid(T)));

		archetypeId.set(componentBit);
		auto newArchetype = GetOrCreateArchetype(archetypeId);

		newArchetype->AddComponent<T>(entityId, component, componentBit);
		newArchetype->MoveComponentsFromOldArchetype(entityCurrentArchetype, entityId);
		m_entityToArchetype[entityId] = newArchetype;
	}

	template<class T>
	T* GetComponent(const EntityId& entityId)
	{
		CheckEntity(entityId);

		if (m_entityToArchetype.find(entityId) == m_entityToArchetype.end())
		{
			TraceLog(LOG_WARNING, "Failed to return Component of unregistered Entity[%d]", entityId);
			return nullptr;
		}
		return m_entityToArchetype[entityId]->GetComponent<T>(entityId);
	}

	template<class T>
	void RemoveComponent(const EntityId& entityId)
	{
		CheckEntity(entityId);

		auto entityCurrentArchetype = m_entityToArchetype[entityId];
		auto archetypeId = entityCurrentArchetype->GetId();
		int componentBit = GetOrCreateComponentBit(std::type_index(typeid(T)));

		archetypeId.set(componentBit, false);
		auto newArchetype = GetOrCreateArchetype(archetypeId);

		entityCurrentArchetype->RemoveComponent<T>(entityId);
		newArchetype->MoveComponentsFromOldArchetype(entityCurrentArchetype, entityId);
		m_entityToArchetype[entityId] = newArchetype;
	}

	template<class ComponentT>
	void RegisterComponentInSystem(System& system)
	{
		int componentBit = GetOrCreateComponentBit(std::type_index(typeid(ComponentT)));
		system.Archetype.set(componentBit);
	}

	std::vector<std::shared_ptr<Archetype>> GetRequiredArchetypes(ArchetypeId requiredArchetypes);

private:
	EntityId m_availableEntityId = 0;
	std::unordered_map<ArchetypeId, std::shared_ptr<Archetype>> m_archetypes;
	std::unordered_map<EntityId, std::shared_ptr<Archetype>> m_entityToArchetype;
	std::unordered_map<std::type_index, int> m_componentTypeToBit;

	std::shared_ptr<Archetype> GetOrCreateArchetype(ArchetypeId archetypeId);
	int GetOrCreateComponentBit(std::type_index componentType);
};
