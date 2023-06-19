#include "raylib.h"
#include "raymath.h"
#include "game.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/GoalComponent.hpp"
#include "Components/ModelComponent.hpp"
#include "Components/LocationComponent.hpp"
#include "Components/TerrainComponent.hpp"

SceneManager::SceneManager()
{}

void SceneManager::InitializeScene()
{
    auto& sceneRenderer = Game::Instance().GetRendererScene();
    auto& ecs = Game::Instance().GetECS();
    auto& gameState = Game::Instance().State;

    if (!m_sceneVariables.SceneModels.empty())
        m_sceneVariables.SceneModels.clear();

    if (!gameState.HumanCities.empty())
        gameState.HumanCities.clear();

    if (!gameState.Humans.empty())
        gameState.Humans.clear();

    if (!gameState.OrcTribes.empty())
        gameState.OrcTribes.clear();

    if (!gameState.Orcs.empty())
        gameState.Orcs.clear();

    if (!gameState.map.empty())
        gameState.map.clear();

    sceneRenderer.InitializeCamera();

    ParseSceneModelInfo();
    LoadModels();

    // add terrain entity
	auto map = ecs.CreateEntity();

	ecs.AddComponent<TransformComponent>(map, {{0, 0, 0}, {0, 0, 0}, 0});
	ecs.AddComponent<ModelComponent>(map, {ModelType::MAP, 1.0f});
	ecs.AddComponent<RenderComponent>(map, {RED, 8.0f});
	ecs.AddComponent<TerrainComponent>(map, {0});

    gameState.map.push_back(map);

    sceneRenderer.InitializeLighting();
	sceneRenderer.ApplyLightingShaderToObjects();
}

void SceneManager::LoadModels()
{
    if (m_sceneFlags.NoModelsMode)
    {        
        m_orcModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
        m_orcModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = LIME;

        m_tribeModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
        m_tribeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = GREEN;

        m_humanModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
        m_humanModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = SKYBLUE;

        m_cityModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
        m_cityModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = BLUE;

        m_mapModel = LoadModelFromMesh(GenMeshPlane(500.0f, 500.0f, 100, 100));
        m_mapModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = RAYWHITE;
        
        m_navmeshModel = LoadModelFromMesh(GenMeshPlane(500.0f, 500.0f, 20, 20));
    }
    else
    {
        if (m_sceneVariables.SceneModels.size() == m_sceneVariables.TotalModelCount)
        {
            m_cityModel = LoadModel(m_sceneVariables.SceneModels[0].c_str());
            m_humanModel = LoadModel(m_sceneVariables.SceneModels[1].c_str());
            m_tribeModel = LoadModel(m_sceneVariables.SceneModels[2].c_str());
            m_orcModel = LoadModel(m_sceneVariables.SceneModels[3].c_str());
            m_mapModel = LoadModel(m_sceneVariables.SceneModels[4].c_str());
            m_navmeshModel = LoadModel(m_sceneVariables.SceneModels[5].c_str());
        }
        else
            TraceLog(LOG_WARNING, "Missing models");

        // This condition means to check if LoadModel() failed and defaulted to cube mesh
        if (m_humanModel.meshCount == 1 && m_humanModel.meshes[0].vertexCount == 24)
        {
            TraceLog(LOG_WARNING, "Failed to load human model");
            m_humanModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
            m_humanModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = SKYBLUE;
        }

        if (m_cityModel.meshCount == 1 && m_cityModel.meshes[0].vertexCount == 24)
        {
            TraceLog(LOG_WARNING, "Failed to load humanCity model");
            m_cityModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
            m_cityModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = BLUE;
        }

        if (m_orcModel.meshCount == 1 && m_orcModel.meshes[0].vertexCount == 24)
        {
            TraceLog(LOG_WARNING, "Failed to load orc model");
            m_orcModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
            m_orcModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = LIME;
        }

        if (m_tribeModel.meshCount == 1 && m_tribeModel.meshes[0].vertexCount == 24)
        {
            TraceLog(LOG_WARNING, "Failed to load orcTribe model");
            m_tribeModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
            m_tribeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = GREEN;
        }

        if (m_mapModel.meshCount == 1 && m_mapModel.meshes[0].vertexCount == 24)
        {
            TraceLog(LOG_WARNING, "Failed to load Map(terrain) model");
            m_mapModel = LoadModelFromMesh(GenMeshPlane(500.0f, 500.0f, 100, 100));
            m_mapModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = RAYWHITE;
        }

        // Perhaps one day this will be generated automatically
        if (m_navmeshModel.meshCount == 1 && m_navmeshModel.meshes[0].vertexCount == 24)
        {
            TraceLog(LOG_WARNING, "Failed to load NavigationMesh model");
            m_navmeshModel = LoadModelFromMesh(GenMeshPlane(500.0f, 500.0f, 20, 20));
        }
    }

    auto& navGrid = Game::Instance().GetNavGrid();
	navGrid.InitializeNavigationGrid(m_navmeshModel);

    m_model_map.insert({ModelType::CITY, &m_cityModel});
    m_model_map.insert({ModelType::HUMAN, &m_humanModel});
    m_model_map.insert({ModelType::TRIBE, &m_tribeModel});
    m_model_map.insert({ModelType::ORC, &m_orcModel});
    m_model_map.insert({ModelType::MAP, &m_mapModel});
    m_model_map.insert({ModelType::NAVMESH, &m_navmeshModel});

    // Map containing ForwardVetor and UpVector for each model type
    for (auto& entry : m_model_map)
    {
        m_modelQuatMap[entry.first].Forward = Vector3Normalize({ 
            entry.second->transform.m2, 
            entry.second->transform.m6, 
            entry.second->transform.m10 });

        m_modelQuatMap[entry.first].Up = Vector3Normalize({ 
            entry.second->transform.m1, 
            entry.second->transform.m5, 
            entry.second->transform.m9 });
    }
}

void SceneManager::ParseSceneModelInfo()
{
    std::fstream file("../resources/Scenes/SceneInfo.txt");

    if (file.is_open())
    {
        std::string line;
        int sceneCounter = -1;
        int safeBreakCounter = 0;

        while (std::getline(file, line))
        {
            if (line.substr(0, 5) == "scene")
            {
                sceneCounter++;
                TraceLog(LOG_INFO, ("../" + line).c_str());
            }

            if (sceneCounter > m_sceneVariables.TotalScenes)
            {
                TraceLog(LOG_WARNING, "Scene number %d not found", m_sceneVariables.SceneNumber);
                m_sceneFlags.NoModelsMode = true;
                return;
            }

            if (sceneCounter == m_sceneVariables.SceneNumber)
            {
                for (int i = 0; i < m_sceneVariables.TotalModelCount; i++) 
                {
                    std::getline(file, line);
                    TraceLog(LOG_INFO, ("../" + line + " found").c_str());
                    m_sceneVariables.SceneModels.push_back("../" + line);
                }

                TraceLog(LOG_INFO, "Scene info loaded");
                return;
            }
        }
    }
    else
    {
        TraceLog(LOG_WARNING, "Could not open scene file, NOMODELS mode On");
        m_sceneFlags.NoModelsMode = true;
        return;
    }
}

void SceneManager::UnloadModels()
{
    m_model_map.clear();

    UnloadModel(m_cityModel);
    UnloadModel(m_humanModel);
    UnloadModel(m_mapModel);
    UnloadModel(m_navmeshModel);
    UnloadModel(m_orcModel);
    UnloadModel(m_tribeModel);
}

SceneManager::~SceneManager()
{
    UnloadModels();
}

Model& SceneManager::GetModel(ModelType model_id)
{
    return *m_model_map[model_id];    
}
