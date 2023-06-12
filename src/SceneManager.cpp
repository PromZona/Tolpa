#include "SceneManager.hpp"

SceneManager::SceneManager()
{}

void SceneManager::LoadModels()
{
    cityModel = LoadModel("../resources/3d_objects/city.glb");
    partyModel = LoadModel("../resources/3d_objects/party.glb");
    mapModel = LoadModel("../resources/3d_objects/TolpaTerrain.glb");
    navmeshModel = LoadModel("../resources/3d_objects/NavigationMesh.glb");

    model_map.insert({ModelType::CITY, &cityModel});
    model_map.insert({ModelType::PARTY, &partyModel});
    model_map.insert({ModelType::MAP, &mapModel});
    model_map.insert({ModelType::NAVMESH, &navmeshModel});
}

SceneManager::~SceneManager()
{
    model_map.clear();

    UnloadModel(cityModel);
    UnloadModel(partyModel);
    UnloadModel(mapModel);
    UnloadModel(navmeshModel);
}

Model& SceneManager::GetModel(ModelType model_id)
{
    return *model_map[model_id];    
}
