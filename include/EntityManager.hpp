#pragma once

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <typeindex>

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
	Entity& GetEntity(int id);
	void DeleteEntity(int id);

	template<typename ComponentType>
	ComponentType& CreateComponent(int& componentId)
	{
		auto& storage = GetComponentStorage<ComponentType>();
		storage.emplace_back(ComponentType{});
		componentId = storage.size() - 1;
		return storage.back();
	}

	template<typename ComponentType>
	std::vector<ComponentType>& GetComponents()
	{
		return GetComponentStorage<ComponentType>();
	}

	template<typename ComponentType>
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
