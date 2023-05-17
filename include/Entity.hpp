#include <string>

class EntityManager;

class Entity
{
public:
    int id;
    string name;
    EntityManager* manager;
    Entity(int id, string name, EntityManager* manager);

    template<class T>
    void AddComponent<T>(T* component);

    template<class T>
    T* GetComponent<T>();
};