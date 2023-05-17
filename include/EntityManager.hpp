#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>

class Entity;
class Component;

class EntityManager
{
public:
    EntityManager();
    ~EntityManager();

    std::shared_ptr<Entity> CreateEntity(int id, String name);
    void DeleteEntity(int id);

    template<class T>
    void AddComponent<T>(std::shared_ptr<T> component);
    template<class T>
    std::shared_ptr<T> GetComponent<T>(int entityId);
private:
    std::vector<std::shared_ptr<Entity>> entities;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> componentsByTypeIndex;
}

