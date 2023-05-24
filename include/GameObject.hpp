#pragma once

#include "raylib.h"
#include <string>

typedef enum
{
    ENTITY,
    TERRAIN,
    BUILDING
} ObjectType;

// 3D model
struct ObjectModel
{
    ObjectModel(); // I dont know why this is needed
    ObjectModel(std::string path_model, std::string path_texture);
    ~ObjectModel();

    // Stores an .glb or .obj file for model mesh
    Model m_model;

    // Baked mesh texture
    Texture2D m_texture;

    // Mesh boundaries (box)
    BoundingBox BBox;
};

// Any GameObject: building, player, enemy etc. should be inherited for specifics
class GameObject
{
public:
    GameObject(std::string path_model, std::string path_texture, ObjectType type, Vector3 position, std::string name);
    ~GameObject();

    ObjectModel* GetObjectModel();
    Vector3 GetObjectPosition();

private:

    // Translation data
    Vector3 m_position;

    std::string m_name;
    int m_object_id;
    
    // Object tag (for different purposes, should be made into different classes probably)
    ObjectType m_type;
    
    // Stores 3D model for this object
    ObjectModel* m_model;
};

