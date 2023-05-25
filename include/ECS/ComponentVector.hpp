#pragma once

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <stdexcept>

#include "raylib.h"
#include "ECSTypes.hpp"

class IComponentVector
{

};

template<class T>
class ComponentVector : public IComponentVector
{
public:
	void AddComponent(const EntityId& entity, const T& component)
	{
		if (m_entityToComponentIndex.find(entity) != m_entityToComponentIndex.end())
		{
			TraceLog(LOG_WARNING, "Failed to add component: Entity[%d] already has component[%s]", entity, typeid(T).name());
			return;
		}
		m_components.push_back(component);
		std::size_t index = m_components.size() - 1;

		m_entityToComponentIndex[entity] = index;
		m_componentToEntityId[index] = entity;
	}

	T& GetComponent(const EntityId& entity)
	{
		if (m_entityToComponentIndex.find(entity) == m_entityToComponentIndex.end())
		{
			TraceLog(LOG_WARNING, "Failed to get component: Entity[%d] has not component[%s]", entity, typeid(T).name());
			throw std::out_of_range("Entity does not have required component");
		}

		return m_components[m_entityToComponentIndex[entity]];
	}

	void RemoveComponent(const EntityId& entity)
	{
		if (m_entityToComponentIndex.find(entity) == m_entityToComponentIndex.end())
		{
			TraceLog(LOG_WARNING, "Failed to remove component: Entity[%d] has not component[%s]", entity, typeid(T).name());
			return;
		}

		std::size_t removableIndex = m_entityToComponentIndex[entity];
		std::size_t lastIndex = m_components.size() - 1;

		m_components[removableIndex] = m_components[lastIndex];
		m_components.pop_back();

		EntityId lastEntity = m_componentToEntityId[lastIndex];
		m_entityToComponentIndex.at(lastEntity) = removableIndex;
		m_componentToEntityId.at(removableIndex) = lastEntity;

		m_entityToComponentIndex.erase(entity);
		m_componentToEntityId.erase(lastIndex);
	}

	std::vector<T>& GetVector()
	{
		return m_components;
	}



private:
	std::vector<T> m_components;
	std::unordered_map<std::size_t, EntityId> m_componentToEntityId;
	std::unordered_map<EntityId, std::size_t> m_entityToComponentIndex;
};
