#pragma once

#include "raylib.h"
#include "SceneManager.hpp"
#include "raymath.h"

#include <stdio.h>
#include <algorithm>

#define MAX_LIGHTS 1         // Max dynamic lights supported by shader

// Light data
typedef struct {   
    int type;
    Vector3 position;
    Vector3 target;
    Color color;
    bool enabled;
    
    // Shader locations
    int enabledLoc;
    int typeLoc;
    int posLoc;
    int targetLoc;
    int colorLoc;
} Light;

// Light type
typedef enum {
    LIGHT_DIRECTIONAL,
    LIGHT_POINT
} LightType;


static int lightsCount = 0;    // Current amount of created lights

// Send light properties to shader
// NOTE: Light shader locations should be available 
void UpdateLightValues(Shader shader, Light light);

// Create a light and get shader locations
Light CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader);

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

    void ApplyLightingShaderToObjects(SceneManager* scene_manager);

    Shader m_shader_light;
    // Ambient light level
    int m_ambient_loc;

    Light m_light;

private:

    Camera m_camera;

    MeshCursorCollisionDetector m_cursor_collision_detector;

    // Basic lighting shader
};
