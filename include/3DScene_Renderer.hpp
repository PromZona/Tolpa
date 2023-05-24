#pragma once

#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>

// For detecting mesh cursor collision, max value of float
#define FLT_MAX     340282346638528859811704183484516925440.0f

#include "raylib.h"

// 3D model
struct RenderedModel
{
    RenderedModel(std::string path_model, std::string path_texture, Vector3 position, std::string name);
    ~RenderedModel();

    std::string m_name;

    Model m_model;
    Texture2D m_texture;
    Vector3 m_position;

    BoundingBox BBox;
};

struct MeshCursorCollisionDetector
{
    // Picking ray
    Ray m_ray = {0};
    RayCollision m_collision;
    RayCollision m_mesh_hit_info;
};

class Scene_Renderer
{

public:

    Scene_Renderer();
    ~Scene_Renderer();
    
    const float m_model_render_scale = 1.0f;

    // Adds new object to render
    void RenderModel(std::string path_model, std::string path_texture, Vector3 position, std::string name);
    
    // Renders all objects and lights
    void RenderScene();

    // Unrenders all objects and clears memory
    void ClearScene();

    void InitializeCamera();

    void RenderCursorRayCollision();

private:


    Camera m_camera;

    bool m_moving_camera = false;

    // Stores all scene models
    std::vector<RenderedModel*> m_scene_models;

    MeshCursorCollisionDetector m_cursor_collision_detector;
};
