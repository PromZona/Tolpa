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

struct RenderDebugVariables
{
    int KDTreeDepthDrawingDepth = 0;
    int KDTreeDepthDrawingStep = 0;
    int KDTreeDepthDrawingElevation = 0;

    float LightRotationSpeed = 0.1f;
};

// GUI interaction for Debug Display
struct RenderFlagsGlobal
{
    bool DrawDebugNavMeshWireframe;
    bool DrawDebugNavMeshGraph;
    bool DrawDebugNavMeshMiddlePoints;
    bool DrawDebugNavMeshKDTree;
    bool DrawDebugNavMeshKDTreeInverted;
    bool DrawDebugNavMeshKDTreeElevated;
    bool DrawDebugTerrainWireframe;
    bool DrawDebugNavMeshNearestPoint;
    bool DrawDebugModels;
    bool RotateLight;
};

struct RenderFlagsUnits
{
    bool DrawDebugPath;
    bool DrawDebugForwardVector;
};

struct RenderFlagsLocations
{
    bool Debug;
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

    // Send light properties to shader
    // NOTE: Light shader locations should be available 
    void UpdateLightValues(Shader shader, Light light);

    // Create a light and get shader locations
    Light CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader);

    // Renders terrain-related objects
    void RenderScene();
    void RotateLight();
    // Location Entities
    
    void InitializeCamera();
    void InitializeLighting();
    void ApplyLightingShaderToObjects();

    inline RenderFlagsGlobal& GetFlags() {return m_GlobalFlags;}
    inline RenderDebugVariables& GetDebugVariables() {return m_DebugVariables;}
    inline Light& GetLight() {return m_light;}
    Camera& GetCamera();

private:

    Shader m_shader_light;
    int m_ambient_loc;
    Light m_light;
    
    RenderFlagsGlobal m_GlobalFlags;
    RenderDebugVariables m_DebugVariables;

    Camera m_camera;
};

class UnitRenderer : public System
{
    public:

    UnitRenderer();
    ~UnitRenderer();

    inline RenderFlagsUnits& GetFlags() {return m_UnitFlags;}

    void RenderUnits();

    private:

    RenderFlagsUnits m_UnitFlags;
};

class LocationRenderer : public System
{
    public:

    LocationRenderer();
    ~LocationRenderer();

    inline RenderFlagsLocations& GetFlags() {return m_LocationFlags;}

    void RenderLocations();

    private:

    RenderFlagsLocations m_LocationFlags;
};

class GuiRenderer : public System
{
    public:

    GuiRenderer();
    ~GuiRenderer();

    void RenderGUI();
    
    private:
};