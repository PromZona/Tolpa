#include "SceneManager.hpp"
#include "raymath.h"
#include "game.hpp"

#define NOMODELSMODE true

SceneManager::SceneManager()
{}

void SceneManager::LoadModels()
{
    #if (!NOMODELSMODE)
    cityModel = LoadModel("../resources/3d_objects/city.glb");
    humanModel = LoadModel("../resources/3d_objects/party.glb");
    mapModel = LoadModel("../resources/3d_objects/TolpaTerrain.glb");
    navmeshModel = LoadModel("../resources/3d_objects/NavigationMesh.glb");
    #else
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
    #endif

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

SceneManager::~SceneManager()
{
    model_map.clear();

    UnloadModel(cityModel);
    UnloadModel(humanModel);
    UnloadModel(mapModel);
    UnloadModel(navmeshModel);
    UnloadModel(orcModel);
    UnloadModel(tribeModel);
}

Model& SceneManager::GetModel(ModelType model_id)
{
    return *model_map[model_id];    
}
