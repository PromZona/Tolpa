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

	friend class Ecs;

	Archetype() = default;
	~Archetype() = default;

	template<typename T>
	std::vector<T>& GetComponents()
	{
		std::type_index componentTypeIndex = std::type_index(typeid(T));
		return std::static_pointer_cast<ComponentVector<T>>(m_componentVectors.at(componentTypeIndex))->GetVector();
	}
private:
	ArchetypeId m_archetypeId;
	std::unordered_map<std::type_index, std::shared_ptr<IComponentVector>> m_componentVectors;

	template<class T>
	void AddComponent(const EntityId& entity, T component)
	{
		std::type_index componentTypeIndex = std::type_index(typeid(T));

		//TODO: Check if component T should be in this archetype

		if (m_componentVectors.find(componentTypeIndex) == m_componentVectors.end())
		{
			m_componentVectors.insert({ componentTypeIndex, std::make_shared<ComponentVector<T>>() });
		}
		std::static_pointer_cast<ComponentVector<T>>(m_componentVectors[componentTypeIndex])->AddComponent(entity, component);
	}

	template<class T>
	void RemoveComponent(const EntityId& entity)
	{
		std::type_index componentTypeIndex = std::type_index(typeid(T));

		if (m_componentVectors.find(componentTypeIndex) == m_componentVectors.end())
		{
			TraceLog(LOG_WARNING, "Failed to remove Component: entity[%d] does not have component[%s]", entity, typeid(T).name());
			return;
		}

		std::static_pointer_cast<ComponentVector<T>>(m_componentVectors[componentTypeIndex])->RemoveComponent(entity);
	}
};
