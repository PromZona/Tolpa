#include "EntityManager.hpp"

#include "Entity.hpp"

EntityManager& EntityManager::instance = EntityManager::Instance();

EntityManager::EntityManager() = default;

EntityManager::~EntityManager() = default;

Entity& EntityManager::CreateEntity(std::string name)
{
    entities.emplace_back(current_entity_index, std::move(name));
    current_entity_index++;
    return entities.back();
}

void EntityManager::DeleteEntity(int id)
{
    for (auto it = entities.begin(); it != entities.end(); ++it)
    {
        if (it->id == id)
        {
            entities.erase(it);
            break;
        }
    }
}

Entity& EntityManager::GetEntity(int id) {
    return entities[id];
}
