#include "../include/GameObject.hpp"

// Shouldnt be ever called (theoretically)
ObjectModel::ObjectModel()
{
    printf("ERR: Object Model standart constructor called\n"); // ..yeah
}

ObjectModel::ObjectModel(std::string path_model, std::string path_texture)
{
    this->m_model = LoadModel(path_model.c_str());
    this->m_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = this->m_texture;
    this->BBox = GetMeshBoundingBox(m_model.meshes[0]);
}

ObjectModel::~ObjectModel()
{
    UnloadTexture(this->m_texture);
    UnloadModel(this->m_model);
}

GameObject::GameObject(std::string path_model, std::string path_texture, ObjectType type, Vector3 position, std::string name)
{
    this->m_model = new ObjectModel(path_model, path_texture);
    this->m_type = type;
    this->m_position = position;
    this->m_name.assign(name);
}

GameObject::~GameObject()
{
    delete this->m_model;
}

ObjectModel* GameObject::GetObjectModel()
{
    return this->m_model;
}

Vector3 GameObject::GetObjectPosition()
{
    return m_position;
}