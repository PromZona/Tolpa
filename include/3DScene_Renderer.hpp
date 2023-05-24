#pragma once

#include "raylib.h"
#include "SceneManager.hpp"
#include "raymath.h"
#include <stdio.h>
#include <algorithm>

// For detecting mesh cursor collision, max value of float
#define FLT_MAX     340282346638528859811704183484516925440.0f

struct MeshCursorCollisionDetector
{
    // Picking ray
    Ray m_ray = {0};
    RayCollision m_collision;
    RayCollision m_mesh_hit_info;
};

class SceneRenderer
{

public:
    SceneRenderer();
    ~SceneRenderer();

    // Dont know if its needed at all
    const float m_model_render_scale = 1.0f;

    // Renders all objects and lights
    void RenderScene(SceneManager* scene_manager);

    void InitializeCamera();

    void RenderCursorRayCollision(SceneManager* scene_manager);

private:

    Camera m_camera;

    MeshCursorCollisionDetector m_cursor_collision_detector;
};
