#pragma once

#include "raylib.h"
#include <unordered_map>

// For assigning and rendering 3d models of entities
enum class ModelType
{
    CITY,
    PARTY,
    LAIR,
    ENEMY,
    MAP,
    NAVMESH
};

class SceneManager
{
public:

    SceneManager();
    ~SceneManager();

    Model& GetModel(ModelType model_id);

    void LoadModels();

private:
    std::unordered_map<ModelType, Model*> model_map;

    Model cityModel;
    Model partyModel;
    Model lairModel;
    Model enemyModel;
    Model mapModel;
    Model navmeshModel;
};
