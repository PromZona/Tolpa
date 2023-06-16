#pragma once

#include "raylib.h"
#include "game.hpp"
#include <iostream>
#include <fstream>
#include <string>
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

struct SceneVariables
{
    int TotalScenes = 3; // Prepared scenes, look at SceneInfo.txt
    int SceneNumber = 0; // Current scene, changed by GuiManager
    int TotalModelCount = 6; // temporary i guess, its just every possible model
    std::vector<std::string> SceneModels;
};

struct SceneFlags
{
    bool NoModelsMode = false;
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
    inline SceneFlags& GetSceneFlags() {return m_sceneFlags;}
    inline SceneVariables& GetSceneVariables() {return m_sceneVariables;}

    void LoadModels();
    void UnloadModels();
    void ParseSceneModelInfo();
    
    void InitializeScene(); 

private:
    std::unordered_map<ModelType, Model*> model_map;
    std::unordered_map<ModelType, ModelQuatVectors> modelQuatMap;

    Model cityModel;
    Model humanModel;
    Model tribeModel;
    Model orcModel;
    Model mapModel;
    Model navmeshModel;

    SceneFlags m_sceneFlags;
    SceneVariables m_sceneVariables;
};
