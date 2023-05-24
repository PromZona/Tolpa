#pragma once

#include "GameObject.hpp"
#include <vector>

class SceneManager
{
public:

    SceneManager();
    ~SceneManager();

    void AddObjectToScene(GameObject* object);
    int CountSceneObjects();

    // id as in object position inside vector
    GameObject* GetSceneObject(int id);

private:
    // Stores all game objects present on this scene
    std::vector<GameObject*> m_scene_objects;
};
