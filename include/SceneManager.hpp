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
    Vector3 Forward;
    Vector3 Up;
};

class SceneManager
{
public:

    SceneManager();
    ~SceneManager();

    Model& GetModel(ModelType model_id);
    inline ModelQuatVectors& GetModelQuaterionVectors(ModelType model_id) {return m_modelQuatMap[model_id];}
    inline std::unordered_map<ModelType, Model*>& GetTypeToModelMap() {return m_model_map;}
    inline SceneFlags& GetSceneFlags() {return m_sceneFlags;}
    inline SceneVariables& GetSceneVariables() {return m_sceneVariables;}

    void LoadModels();
    void UnloadModels();
    void ParseSceneModelInfo();
    
    void InitializeScene(); 

private:
    std::unordered_map<ModelType, Model*> m_model_map;
    std::unordered_map<ModelType, ModelQuatVectors> m_modelQuatMap;


    Model m_cityModel;
    Model m_humanModel;
    Model m_tribeModel;
    Model m_orcModel;
    Model m_mapModel;
    Model m_navmeshModel;

    SceneFlags m_sceneFlags;
    SceneVariables m_sceneVariables;
};
