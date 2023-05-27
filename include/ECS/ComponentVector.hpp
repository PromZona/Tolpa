#pragma once

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <stdexcept>
#include <memory>

#include "raylib.h"
#include "ECSTypes.hpp"

class IComponentVector
{
public:
	virtual ~IComponentVector() = default;
	virtual void MoveComponents(const EntityId& entity, std::shared_ptr<IComponentVector> newVector) = 0;
	virtual int GetComponentBit() = 0;
	virtual void PrintInfo() = 0;
	virtual std::shared_ptr<IComponentVector> CreateEmptyClone() const = 0;
};

template<class T>
class ComponentVector : public IComponentVector
{
public:
	ComponentVector() : m_componentBit(0), m_entityToComponentIndex(), m_components(), m_componentToEntityId(){};
	ComponentVector(int componentBit) : m_componentBit(componentBit), m_entityToComponentIndex(), m_components(), m_componentToEntityId(){};
	~ComponentVector() override = default;

	void AddComponent(const EntityId& entity, const T& component)
	{
		if (m_entityToComponentIndex.count(entity) == 1)
		{
			TraceLog(LOG_WARNING, "Failed to add component: Entity[%d] already has component[%s]", entity, typeid(T).name());
			return;
		}
		std::size_t index = m_components.size();
		m_components.push_back(component);

		m_entityToComponentIndex[entity] = index;
		m_componentToEntityId[index] = entity;
	}

	std::unique_ptr<T> GetComponent(const EntityId& entity)
	{
		if (m_entityToComponentIndex.find(entity) == m_entityToComponentIndex.end())
		{
			TraceLog(LOG_WARNING, "Failed to get component: Entity[%d] has no component[%s]", entity, typeid(T).name());
			return nullptr;
		}

		return std::make_unique<T>(m_components[m_entityToComponentIndex[entity]]);
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

	void MoveComponents(const EntityId& entity, std::shared_ptr<IComponentVector> newVector) override
	{
		auto comp = GetComponent(entity);
		std::shared_ptr<ComponentVector<T>> ptr = std::static_pointer_cast<ComponentVector<T>>(newVector);
		ptr->AddComponent(entity,*comp);
		RemoveComponent(entity);
	}

	int GetComponentBit() override
	{
		return m_componentBit;
	}

	void PrintInfo() override
	{
		TraceLog(LOG_INFO, "\t\t[ComponentVec] Bit: %d. Count %d", m_componentBit, m_components.size());
		for (auto& data : m_entityToComponentIndex)
		{
			TraceLog(LOG_INFO, "\t\t\t%d -> %d [Entity -> ComponentId]", data.first, data.second);
		}
	}

	std::shared_ptr<IComponentVector> CreateEmptyClone() const
	{
		// Returns an empty ComponentVector that matches this one's type
		return std::make_shared<ComponentVector<T>>(m_componentBit);
	}

private:
	int m_componentBit;
	std::vector<T> m_components;
	std::unordered_map<std::size_t, EntityId> m_componentToEntityId;
	std::unordered_map<EntityId, std::size_t> m_entityToComponentIndex;

};
