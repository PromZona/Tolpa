#include "ECS/ECS.hpp"

ECS::ECS() : m_availableEntityId(0) {}
ECS::~ECS() = default;

EntityId ECS::CreateEntity()
{
	EntityId result = m_availableEntityId;
	m_availableEntityId++;
	m_entityToArchetype.insert({result, GetOrCreateArchetype(0)});
	return result;
}

void ECS::DeleteEntity(const EntityId& entityId)
{
	m_entityToArchetype.erase(entityId);
}

std::shared_ptr<Archetype> ECS::GetOrCreateArchetype(ArchetypeId archetypeId)
{
	if (m_archetypes.find(archetypeId) == m_archetypes.end())
		m_archetypes[archetypeId] = std::make_shared<Archetype>(archetypeId);

	return m_archetypes[archetypeId];
}

int ECS::GetOrCreateComponentBit(std::type_index componentType)
{
	if (m_componentTypeToBit.find(componentType) == m_componentTypeToBit.end())
	{
		if (m_componentTypeToBit.size() >= MAX_COMPONENTS - 1)
		{
			TraceLog(LOG_WARNING, "Failed to add new Component. Capacity Reached");
			throw std::out_of_range("Capacity of components reached");
		}

		m_componentTypeToBit[componentType] = (int)m_componentTypeToBit.size();
	}

	return m_componentTypeToBit[componentType];
}

void ECS::PrintInfo()
{
	TraceLog(LOG_INFO, "[ECS] Entities: %d", m_availableEntityId);
	TraceLog(LOG_INFO, "[ECS] Components To Bit");
	for (auto& data : m_componentTypeToBit)
	{
		TraceLog(LOG_INFO, "\t %s -> %d", data.first.name(), data.second);
	}

	TraceLog(LOG_INFO, "[ECS] Entity To Archetype");
	for (auto& data : m_entityToArchetype)
	{
		TraceLog(LOG_INFO, "\t %d -> %s", data.first, data.second->GetId().to_string().data());
	}

	TraceLog(LOG_INFO, "[ECS] Archetypes");
	for (auto& data : m_archetypes)
	{
		data.second->PrintInfo();
	}
}

