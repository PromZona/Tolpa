#include "SceneManager.hpp"

SceneManager::SceneManager()
{}

void SceneManager::LoadModels()
{
    cityModel = LoadModel("../resources/3d_objects/city.glb");
    partyModel = LoadModel("../resources/3d_objects/party.glb");
    mapModel = LoadModel("../resources/3d_objects/Tolpa_terrain.glb");

    model_map.insert({ModelType::CITY, &cityModel});
    model_map.insert({ModelType::PARTY, &partyModel});
    model_map.insert({ModelType::MAP, &mapModel});
}

SceneManager::~SceneManager()
{
    model_map.clear();

    UnloadModel(cityModel);
    UnloadModel(partyModel);
    UnloadModel(mapModel);
}

Model& SceneManager::GetModel(ModelType model_id)
{
    return *model_map[model_id];    
}
