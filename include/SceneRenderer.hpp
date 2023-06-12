#pragma once

#include <stdio.h>
#include <algorithm>

#include "raylib.h"
#include "raymath.h"

#include "ECS/System.hpp"

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

// GUI interaction for Debug Display
struct RenderFlags
{
    bool drawDebugNavMeshWireframe;
    bool drawDebugNavMeshMidConnect;
    bool drawDebugTerrainWireframe;
    bool drawDebugBoundingBoxes;
};

static int lightsCount = 0;    // Current amount of created lights

struct MeshCursorCollisionDetector
{
    // Picking ray
    Ray m_ray = {0};
    RayCollision m_collision;
    RayCollision m_mesh_hit_info;
};

class SceneRenderer : public System
{

public:
    SceneRenderer();
    ~SceneRenderer();

    // Dont know if its needed at all
    const float m_model_render_scale = 1.0f;

    // Send light properties to shader
    // NOTE: Light shader locations should be available 
    void UpdateLightValues(Shader shader, Light light);

    // Create a light and get shader locations
    Light CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader);

    // Renders all objects and lights
    void RenderScene();
    void InitializeCamera();
    void InitializeLighting();
    void ApplyLightingShaderToObjects();

    inline RenderFlags& GetFlags() {return m_flags;}

    Camera& GetCamera();


private:

    Shader m_shader_light;
    int m_ambient_loc;
    Light m_light;
    RenderFlags m_flags;
    Camera m_camera;
    MeshCursorCollisionDetector m_cursor_collision_detector;

    // Basic lighting shader
};
