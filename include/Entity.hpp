#pragma once

#include <string>
#include <memory>
#include <utility>

#include "EntityManager.hpp"

class Entity
{
public:
    int id;
    std::string name;
    Entity(int id, std::string name) : id(id), name(std::move(name)){}
    ~Entity() = default;

    template<class T>
    T& AddComponent()
    {
        auto& entityManager = EntityManager::Instance();
        int componentId;
        auto& result = entityManager.CreateComponent<T>(componentId);

        componentToId[std::type_index(typeid(T))] = componentId;
        return result;
    }

    template<class T>
    T& GetComponent()
    {
        auto& entityManager = EntityManager::Instance();
        return entityManager.GetComponent<T>(componentToId[std::type_index(typeid(T))]);
    }

private:
    std::unordered_map<std::type_index, int> componentToId;
};

