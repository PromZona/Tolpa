#pragma once

#include "raylib.h"
#include <unordered_map>

// For assigning and rendering 3d models of entities
enum class ModelType
{
    CITY,
    HUMAN,
    TRIBE,
    ORC,
    MAP,
    NAVMESH
};

struct ModelQuatVectors
{
    Vector3 forward;
    Vector3 up;
};

class SceneManager
{
public:

    SceneManager();
    ~SceneManager();

    Model& GetModel(ModelType model_id);
    inline ModelQuatVectors& GetModelQuaterionVectors(ModelType model_id) {return modelQuatMap[model_id];}
    inline std::unordered_map<ModelType, Model*>& GetTypeToModelMap() {return model_map;}

    void LoadModels();

private:
    std::unordered_map<ModelType, Model*> model_map;
    std::unordered_map<ModelType, ModelQuatVectors> modelQuatMap;

    Model cityModel;
    Model humanModel;
    Model tribeModel;
    Model orcModel;
    Model mapModel;
    Model navmeshModel;
};
