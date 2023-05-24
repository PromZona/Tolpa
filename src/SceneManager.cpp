#include "../include/SceneManager.hpp"

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{
    // Unloading all objects on the scene
    for (size_t i = 0; i < this->m_scene_objects.size(); i++)
        delete this->m_scene_objects[i];
}

void SceneManager::AddObjectToScene(GameObject* object)
{
    // Object is stored in scene vector
    this->m_scene_objects.push_back(object);
}

int SceneManager::CountSceneObjects()
{
    return this->m_scene_objects.size();
}

GameObject* SceneManager::GetSceneObject(int id)
{
    return this->m_scene_objects[id];
}