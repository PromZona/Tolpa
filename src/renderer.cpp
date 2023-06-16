#include "../include/Renderer.hpp"

#include "Components/RenderComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/ModelComponent.hpp"
#include "Components/GoalComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/LocationComponent.hpp"
#include "Components/TerrainComponent.hpp"

#include "game.hpp"
#include "rlgl.h"

SceneRenderer::SceneRenderer(){}
UnitRenderer::UnitRenderer(){}
LocationRenderer::LocationRenderer(){}
GuiRenderer::GuiRenderer(){}

SceneRenderer::~SceneRenderer()
{
    UnloadShader(m_shader_light);
}
UnitRenderer::~UnitRenderer(){}
LocationRenderer::~LocationRenderer(){}
GuiRenderer::~GuiRenderer(){}

void SceneRenderer::InitializeCamera()
{
    m_camera = { 0 };
    m_camera.position = {400.0f, 400.0f, 400.0f};
    m_camera.target = {0.0f, 0.0f, 0.0f};
    m_camera.up = {0.0f, 1.0f, 0.0f};
    m_camera.fovy = 45.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

Camera& SceneRenderer::GetCamera()
{
    return m_camera;
}

void SceneRenderer::InitializeLighting()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    m_shader_light = LoadShader("../resources/shaders/lighting.vs",
                                "../resources/shaders/lighting.fs");

    m_shader_light.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(m_shader_light, "viewPos");
    m_ambient_loc = GetShaderLocation(m_shader_light, "ambient");
    
    float temp[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    SetShaderValue(m_shader_light, m_ambient_loc, temp, SHADER_UNIFORM_VEC4);

    m_light = CreateLight(LIGHT_POINT, {0, 150, 0}, Vector3Zero(), WHITE, m_shader_light);
}

void SceneRenderer::ApplyLightingShaderToObjects()
{
    auto& sceneManager = Game::Instance().GetSceneManager();

    auto& model_map = sceneManager.GetTypeToModelMap();

    for (auto& entry : model_map)
    {
        model_map[entry.first]->materials[0].shader = m_shader_light;
    }
}

Light SceneRenderer::CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader)
{
    Light light = { 0 };

    light.enabled = true;
    light.type = type;
    light.position = position;
    light.target = target;
    light.color = color;

    // For setting uniform values in fragment shader
    char enabledName[32] = "sc_light.enabled\0";
    char typeName[32] = "sc_light.type\0";
    char posName[32] = "sc_light.position\0";
    char targetName[32] = "sc_light.target\0";
    char colorName[32] = "sc_light.color\0";
    
    light.enabledLoc = GetShaderLocation(shader, enabledName);
    light.typeLoc = GetShaderLocation(shader, typeName);
    light.posLoc = GetShaderLocation(shader, posName);
    light.targetLoc = GetShaderLocation(shader, targetName);
    light.colorLoc = GetShaderLocation(shader, colorName);

    UpdateLightValues(shader, light);

    return light;
}

void SceneRenderer::UpdateLightValues(Shader shader, Light light)
{
    // Send to shader light enabled state and type
    SetShaderValue(shader, light.enabledLoc, &light.enabled, SHADER_UNIFORM_INT);
    SetShaderValue(shader, light.typeLoc, &light.type, SHADER_UNIFORM_INT);

    // Send to shader light position values
    float position[3] = { light.position.x, light.position.y, light.position.z };
    SetShaderValue(shader, light.posLoc, position, SHADER_UNIFORM_VEC3);

    // Send to shader light target position values
    float target[3] = { light.target.x, light.target.y, light.target.z };
    SetShaderValue(shader, light.targetLoc, target, SHADER_UNIFORM_VEC3);

    // Send to shader light color values
    float color[4] = { (float)light.color.r/(float)255, (float)light.color.g/(float)255, 
                       (float)light.color.b/(float)255, (float)light.color.a/(float)255 };
    SetShaderValue(shader, light.colorLoc, color, SHADER_UNIFORM_VEC4);
}

// Scene-specific Renders
// Camera updates
// Light updates
// Terrain rendering
void SceneRenderer::RenderScene()
{
    auto& ecs = Game::Instance().GetECS();
    auto& SceneManager = Game::Instance().GetSceneManager();
    auto& NavGrid = Game::Instance().GetNavGrid();
	auto archetypes = ecs.GetRequiredArchetypes(Archetype);
    
    float cameraPos[3] = {m_camera.position.x, m_camera.position.y, m_camera.position.z};
    SetShaderValue(m_shader_light, m_shader_light.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

    if (IsKeyPressed(KEY_B))
        m_light.enabled = !m_light.enabled;

    UpdateLightValues(m_shader_light, m_light);

    for (auto& archetype : archetypes)
    {
        auto& transforms = archetype->GetComponents<TransformComponent>();
        auto& renders = archetype->GetComponents<RenderComponent>();
        auto& models = archetype->GetComponents<ModelComponent>();
        auto& terrain = archetype->GetComponents<TerrainComponent>(); // No use currently

        std::size_t size = transforms.size();

        for (std::size_t i = 0; i < size; i++)
        {
            Model currentModel = SceneManager.GetModel(models[i].model_id);

            if (models[i].model_id == ModelType::MAP && m_GlobalFlags.drawDebugTerrainWireframe)
            {
                DrawModelWires(currentModel, transforms[i].Position, models[i].scale, WHITE);
                continue;
            }

            //DrawModel(currentModel, transforms[i].Position, models[i].scale, WHITE);
        }
    }

    if (m_GlobalFlags.drawDebugNavMeshMiddlePoints)
    {
        auto& navGraphNodes = NavGrid.GetGraphNodes();

        for (int i = 0; i < navGraphNodes.size(); i++)
            DrawPoint3D(navGraphNodes[i], GREEN);
    }

    if (m_GlobalFlags.drawDebugNavMeshGraph)
        NavGrid.DebugDrawNavMeshGraph();

    if (m_GlobalFlags.drawDebugNavMeshWireframe)
        NavGrid.DebugDrawWireframe();

    if (m_GlobalFlags.drawDebugNavMeshKDTree)
    {
        auto& kd_tree = NavGrid.GetNavKDTree();

        if (m_GlobalFlags.drawDebugNavMeshKDTreeInverted)
            kd_tree.DebugDrawTreeByDepth(*kd_tree.GetRoot(), 
            m_DebugVariables.KDTreeDepthDrawingDepth,
            m_GlobalFlags.drawDebugNavMeshKDTreeInverted,
            m_GlobalFlags.drawDebugNavMeshKDTreeElevated);
        else
            kd_tree.DebugDrawTreeByDepth(*kd_tree.GetRoot(), 
            m_DebugVariables.KDTreeDepthDrawingDepth, 0,
            m_GlobalFlags.drawDebugNavMeshKDTreeElevated);
    }
    if (m_GlobalFlags.drawDebugNavMeshNearestPoint)
    {
        auto& testPoint = NavGrid.GetTestPoint();        
        DrawSphereWires(testPoint, 5.0f, 10, 10, ORANGE);

        NavGrid.DebugDrawNearestNeighbour(&m_camera);
    }

    DrawSphereEx(m_light.position, 0.5f, 8, 8, m_light.color);
}

// EntityUnit-Specific Renders
// Unit model rendering
// Path drawing
// Model Rotation (TODO)
void UnitRenderer::RenderUnits()
{
    auto& ecs = Game::Instance().GetECS();
    auto& SceneManager = Game::Instance().GetSceneManager();
    auto& NavGrid = Game::Instance().GetNavGrid();
	auto archetypes = ecs.GetRequiredArchetypes(Archetype);

    for (auto& archetype : archetypes)
    {
        auto& c_transforms = archetype->GetComponents<TransformComponent>();
        auto& c_renders = archetype->GetComponents<RenderComponent>();
        auto& c_models = archetype->GetComponents<ModelComponent>();
        auto& c_goals = archetype->GetComponents<GoalComponent>();
        auto& c_movement = archetype->GetComponents<MovementComponent>();

        std::size_t size = c_transforms.size();

        for (std::size_t i = 0; i < size; i++)
        {
            Model currentModel = SceneManager.GetModel(c_models[i].model_id);
            DrawModel(currentModel, c_transforms[i].Position, c_models[i].scale, WHITE);

            if (m_UnitFlags.drawDebugPath)
                NavGrid.DebugDrawPath(c_goals[i].PathToGoal, c_goals[i].steps);
        }
    }
}

// Location-Specific Renders
// Location model rendering
void LocationRenderer::RenderLocations()
{
    auto& ecs = Game::Instance().GetECS();
    auto& SceneManager = Game::Instance().GetSceneManager();
    auto& NavGrid = Game::Instance().GetNavGrid();
	auto archetypes = ecs.GetRequiredArchetypes(Archetype);

    for (auto& archetype : archetypes)
    {
        auto& c_transforms = archetype->GetComponents<TransformComponent>();
        auto& c_renders = archetype->GetComponents<RenderComponent>();
        auto& c_models = archetype->GetComponents<ModelComponent>();
        auto& c_location = archetype->GetComponents<LocationComponent>(); // No use currently

        std::size_t size = c_transforms.size();

        for (std::size_t i = 0; i < size; i++)
        {
            Model currentModel = SceneManager.GetModel(c_models[i].model_id);
            DrawModel(currentModel, c_transforms[i].Position, c_models[i].scale, WHITE);
        }
    }
}

// Everything that needs to be rendered in 2d
void GuiRenderer::RenderGUI()
{
    Game::Instance().GetGUI().DrawGUI();

    DrawText("Controls:", 10, 35, 20, DARKGRAY);
    DrawText("[C] - Create City", 10, 60, 20, DARKGRAY);
    DrawText("[H] - Create Human", 10, 85, 20, DARKGRAY);
    DrawText("[T] - Create Tribe", 10, 110, 20, DARKGRAY);
    DrawText("[O] - Create Orc", 10, 135, 20, DARKGRAY);

}