#pragma once

#include <vector>
#include <map>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <concepts>

class Entity;


class EntityManager
{
public:
    static EntityManager& Instance()
    {
        static EntityManager instance;
        return instance;
    }

    ~EntityManager();

    Entity& CreateEntity(std::string name);
    void DeleteEntity(int id);

    template <typename ComponentType>
    ComponentType& CreateComponent(int& componentId)
    {
        auto& storage = GetComponentStorage<ComponentType>();
        storage.emplace_back(ComponentType{});
        componentId = storage.size();
        return storage.back();
    }

    template <typename ComponentType>
    std::vector<ComponentType>& GetComponents()
    {
        return GetComponentStorage<ComponentType>();
    }

    template <typename ComponentType>
    ComponentType& GetComponent(int componentId)
    {
        return GetComponentStorage<ComponentType>()[componentId];
    }

private:
    EntityManager();
    static EntityManager& instance;

    int current_entity_index = 0;
    std::vector<Entity> entities;

    template<typename Component>
    std::vector<Component>& GetComponentStorage()
    {
        static std::vector<Component> components;
        return components;
    }
};
