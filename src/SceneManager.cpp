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
        orcModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
        orcModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = LIME;

        tribeModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
        tribeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = GREEN;

        humanModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
        humanModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = SKYBLUE;

        cityModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
        cityModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = BLUE;

        mapModel = LoadModelFromMesh(GenMeshPlane(500.0f, 500.0f, 100, 100));
        mapModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = RAYWHITE;
        
        navmeshModel = LoadModelFromMesh(GenMeshPlane(500.0f, 500.0f, 20, 20));
    }
    else
    {
        if (m_sceneVariables.SceneModels.size() == m_sceneVariables.TotalModelCount)
        {
            cityModel = LoadModel(m_sceneVariables.SceneModels[0].c_str());
            humanModel = LoadModel(m_sceneVariables.SceneModels[1].c_str());
            tribeModel = LoadModel(m_sceneVariables.SceneModels[2].c_str());
            orcModel = LoadModel(m_sceneVariables.SceneModels[3].c_str());
            mapModel = LoadModel(m_sceneVariables.SceneModels[4].c_str());
            navmeshModel = LoadModel(m_sceneVariables.SceneModels[5].c_str());
        }
        else
            TraceLog(LOG_WARNING, "Missing models");

        if (!IsModelReady(humanModel))
        {
            TraceLog(LOG_WARNING, "Failed to load human model");
            humanModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
            humanModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = SKYBLUE;
        }

        if (!IsModelReady(cityModel))
        {
            TraceLog(LOG_WARNING, "Failed to load humanCity model");
            cityModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
            cityModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = BLUE;
        }

        if (!IsModelReady(orcModel))
        {
            TraceLog(LOG_WARNING, "Failed to load orc model");
            orcModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
            orcModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = LIME;
        }

        if (!IsModelReady(tribeModel))
        {
            TraceLog(LOG_WARNING, "Failed to load orcTribe model");
            tribeModel = LoadModelFromMesh(GenMeshCylinder(10.0f, 10.0f, 10));
            tribeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = GREEN;
        }

        if (!IsModelReady(mapModel))
        {
            TraceLog(LOG_WARNING, "Failed to load Map(terrain) model");
            mapModel = LoadModelFromMesh(GenMeshPlane(500.0f, 500.0f, 100, 100));
            mapModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = RAYWHITE;
        }

        // Perhaps one day this will be generated automatically
        if (!IsModelReady(navmeshModel))
        {
            TraceLog(LOG_WARNING, "Failed to load NavigationMesh model");
            navmeshModel = LoadModelFromMesh(GenMeshPlane(500.0f, 500.0f, 20, 20));
        }
    }

    auto& navGrid = Game::Instance().GetNavGrid();
	navGrid.InitializeNavigationGrid(navmeshModel);

    model_map.insert({ModelType::CITY, &cityModel});
    model_map.insert({ModelType::HUMAN, &humanModel});
    model_map.insert({ModelType::TRIBE, &tribeModel});
    model_map.insert({ModelType::ORC, &orcModel});
    model_map.insert({ModelType::MAP, &mapModel});
    model_map.insert({ModelType::NAVMESH, &navmeshModel});

    // Map containing ForwardVetor and UpVector for each model type
    for (auto& entry : model_map)
    {
        modelQuatMap[entry.first].forward = Vector3Normalize({ 
            entry.second->transform.m2, 
            entry.second->transform.m6, 
            entry.second->transform.m10 });

        modelQuatMap[entry.first].up = Vector3Normalize({ 
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
    model_map.clear();

    UnloadModel(cityModel);
    UnloadModel(humanModel);
    UnloadModel(mapModel);
    UnloadModel(navmeshModel);
    UnloadModel(orcModel);
    UnloadModel(tribeModel);
}

SceneManager::~SceneManager()
{
    UnloadModels();
}

Model& SceneManager::GetModel(ModelType model_id)
{
    return *model_map[model_id];    
}
