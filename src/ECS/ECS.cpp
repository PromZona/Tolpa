#include "ECS/ECS.hpp"

ECS::ECS() {}
ECS::~ECS() {}

EntityId ECS::CreateEntity()
{
	EntityId result = m_availableEntityId;
	m_availableEntityId++;
	m_entityToArchetype[result] = Archetype();
	return result;
}

void ECS::DeleteEntity(const EntityId& entityId)
{
	m_entityToArchetype.erase(entityId);
}

