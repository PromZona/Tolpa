#pragma once

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "ComponentVector.hpp"
#include "ECSTypes.hpp"

class Archetype
{
public:

	friend class ECS;

	Archetype(ArchetypeId id) : m_archetypeId(id) {};
	~Archetype() = default;

	ArchetypeId GetId()
	{
		return m_archetypeId;
	}

	void MoveComponentsFromOldArchetype(const std::shared_ptr<Archetype>& oldArchetype, const EntityId& entity)
	{
		for (auto& pair : oldArchetype->m_componentVectors)
		{
			if (m_archetypeId.test(pair.second->GetComponentBit()))
			{
				auto it = m_componentVectors.find(pair.first);
				if (it == m_componentVectors.end())
					m_componentVectors.insert({pair.first, pair.second->CreateEmptyClone()});
				pair.second->MoveComponents(entity, m_componentVectors[pair.first]);
			}
		}
	}

	void PrintInfo()
	{
		TraceLog(LOG_INFO, "\t[Archetype] Archetype vectors %s", m_archetypeId.to_string().data());
		for (auto& data : m_componentVectors)
		{
			data.second->PrintInfo();
		}
	}

	template<typename T>
	std::vector<T>& GetComponents()
	{
		auto componentTypeIndex = std::type_index(typeid(T));
		return std::static_pointer_cast<ComponentVector<T>>(m_componentVectors.at(componentTypeIndex))->GetVector();
	}

	std::unordered_map<std::type_index, std::shared_ptr<IComponentVector>> GetRawComponents()
	{
		return m_componentVectors;
	}
private:
	ArchetypeId m_archetypeId;
	std::unordered_map<std::type_index, std::shared_ptr<IComponentVector>> m_componentVectors;

	template<class T>
	void AddComponent(const EntityId& entity, T component, int componentBit)
	{
		auto componentTypeIndex = std::type_index(typeid(T));

		//TODO: Check if component T should be in this archetype

		if (m_componentVectors.find(componentTypeIndex) == m_componentVectors.end())
		{
			m_componentVectors.insert({ componentTypeIndex, std::make_shared<ComponentVector<T>>(componentBit) });
		}
		std::static_pointer_cast<ComponentVector<T>>(m_componentVectors[componentTypeIndex])->AddComponent(entity, component);
	}

	template<class T>
	T* GetComponent(const EntityId& entity)
	{
		auto componentTypeIndex = std::type_index(typeid(T));
		if (m_componentVectors.find(componentTypeIndex) == m_componentVectors.end())
		{
			TraceLog(LOG_WARNING, "Failed to get Component: archetype[%d] does not have component[%s]", m_archetypeId.to_ulong(), typeid(T).name());
			return nullptr;
		}
		return std::static_pointer_cast<ComponentVector<T>>(m_componentVectors[componentTypeIndex])->GetComponent(entity);
	}

	template<class T>
	void RemoveComponent(const EntityId& entity)
	{
		auto componentTypeIndex = std::type_index(typeid(T));

		if (m_componentVectors.find(componentTypeIndex) == m_componentVectors.end())
		{
			TraceLog(LOG_WARNING, "Failed to remove Component: archetype[%d] does not have component[%s]", m_archetypeId.to_ulong(), typeid(T).name());
			return;
		}

		std::static_pointer_cast<ComponentVector<T>>(m_componentVectors[componentTypeIndex])->RemoveComponent(entity);
	}
};
