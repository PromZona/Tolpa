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
    TraceLog(LOG_INFO, "Initializing Camera...");
    m_camera = { 0 };
    m_camera.position = {400.0f, 400.0f, 400.0f};
    m_camera.target = {0.0f, 0.0f, 0.0f};
    m_camera.up = {0.0f, 1.0f, 0.0f};
    m_camera.fovy = 90.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

Camera& SceneRenderer::GetCamera()
{
    return m_camera;
}

void SceneRenderer::InitializeLighting()
{
    if(!IsShaderReady(m_shader_light))
    {
        TraceLog(LOG_INFO, "Loading Shader...");
        SetConfigFlags(FLAG_MSAA_4X_HINT);
        m_shader_light = LoadShader("../resources/shaders/lighting.vs",
                                    "../resources/shaders/lighting.fs");

        m_shader_light.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(m_shader_light, "viewPos");
        m_ambient_loc = GetShaderLocation(m_shader_light, "ambient");
        
        float temp[4] = {0.1f, 0.1f, 0.1f, 1.0f};
        SetShaderValue(m_shader_light, m_ambient_loc, temp, SHADER_UNIFORM_VEC4);

        m_light = CreateLight(LIGHT_POINT, {0, 150, 0}, Vector3Zero(), WHITE, m_shader_light);
    }
    else
        TraceLog(LOG_INFO, "Shader already loaded...");
}
void SceneRenderer::ApplyLightingShaderToObjects()
{
    TraceLog(LOG_INFO, "Applying shader to scene models...");

    auto& sceneManager = Game::Instance().GetSceneManager();

    auto& model_map = sceneManager.GetTypeToModelMap();

    for (auto& entry : model_map)
    {
        for (int i = 0; i < model_map[entry.first]->materialCount; i++)
            model_map[entry.first]->materials[i].shader = m_shader_light;
    }
}

Light SceneRenderer::CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader)
{
    TraceLog(LOG_INFO, "Creating light...");
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

void SceneRenderer::RotateLight()
{
    // Convert the angle from degrees to radians
    double radians = m_DebugVariables.LightRotationSpeed * PI / 180.0;

    // Rotation matrix around the Y-axis
    double cosTheta = cos(radians);
    double sinTheta = sin(radians);

    m_light.position.x = m_light.position.x * cosTheta + m_light.position.z * sinTheta;
    m_light.position.y = m_light.position.y;
    m_light.position.z = -m_light.position.x * sinTheta + m_light.position.z * cosTheta;
}

void SceneRenderer::RenderPlacingPreview()
{
    auto& hitPoint = m_meshPicker.GetCollisionPoint();
    auto& sceneManager = Game::Instance().GetSceneManager();

    float previewScale = 1.0f;
    switch (m_gameplayVariables.previewModelType)
    {
        case ModelType::HUMAN:
            previewScale = 0.1f;
            break;
        case ModelType::CITY:
            previewScale = 1.0f;
            break;
        case ModelType::TRIBE:
            previewScale = 1.0f;
            break;
        default:
            break;
    }

    DrawModelWires(sceneManager.GetModel(m_gameplayVariables.previewModelType), hitPoint.point, previewScale, WHITE);    
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

            if (models[i].model_id == ModelType::MAP && m_GlobalFlags.DrawDebugTerrainWireframe)
            {
                DrawModelWires(currentModel, transforms[i].Position, models[i].scale, WHITE);
                continue;
            }

            if (m_GlobalFlags.DrawDebugModels)
                DrawModel(currentModel, transforms[i].Position, models[i].scale, WHITE);
        }
    }

    if (m_gameplayVariables.ShowPlacingPreview)
    {
        m_meshPicker.GetTerrainHit();
        RenderPlacingPreview();
    }

    if (m_GlobalFlags.DrawDebugNavMeshMiddlePoints)
    {
        auto& navGraphNodes = NavGrid.GetGraphNodes();

        for (int i = 0; i < navGraphNodes.size(); i++)
            DrawPoint3D(navGraphNodes[i], GREEN);
    }

    if (m_GlobalFlags.DrawDebugNavMeshGraph)
        NavGrid.DebugDrawNavMeshGraph();

    if (m_GlobalFlags.DrawDebugNavMeshWireframe)
        NavGrid.DebugDrawWireframe();

    if (m_GlobalFlags.DrawDebugNavMeshKDTree)
    {
        auto& kd_tree = NavGrid.GetNavKDTree();

        if (m_GlobalFlags.DrawDebugNavMeshKDTreeInverted)
            kd_tree.DebugDrawTreeByDepth(*kd_tree.GetRoot(), 
            m_DebugVariables.KDTreeDepthDrawingDepth,
            m_GlobalFlags.DrawDebugNavMeshKDTreeInverted,
            m_GlobalFlags.DrawDebugNavMeshKDTreeElevated);
        else
            kd_tree.DebugDrawTreeByDepth(*kd_tree.GetRoot(), 
            m_DebugVariables.KDTreeDepthDrawingDepth, 0,
            m_GlobalFlags.DrawDebugNavMeshKDTreeElevated);
    }

    if (m_GlobalFlags.DrawDebugNavMeshNearestPoint)
    {
        auto& testPoint = NavGrid.GetTestPoint();        
        DrawSphereWires(testPoint, 5.0f, 10, 10, ORANGE);

        NavGrid.DebugDrawNearestNeighbour(&m_camera);
    }
    
    if (m_GlobalFlags.RotateLight)
        RotateLight();

    DrawSphereEx(m_light.position, 0.5f, 8, 8, m_light.color);
}

// rotation axis and angle for model to face the destination point
std::tuple<Vector3, float> GetRotationTowardsPoint(Vector3 forward, Vector3 up, Vector3 position, Vector3 destination)
{
    Vector3 desiredForward = Vector3Normalize(Vector3Subtract(destination, position));

    Vector3 rotationAxis = Vector3CrossProduct(forward, desiredForward);
    rotationAxis = Vector3Normalize(rotationAxis);

    float angle = acos(Vector3DotProduct(forward, desiredForward));
    float angleDegrees = angle * (180.0f / PI);

    // Model will only rotate around Y axis, otherwise it will go crazy on steep surface
    rotationAxis.x = 0.0f;
    rotationAxis.z = 0.0f;

    if (rotationAxis.y < 0)
        angleDegrees = -angleDegrees;

    return std::make_tuple(rotationAxis, angleDegrees);        
}

void ModelRotationTick(float& angle, float& tempo)
{
    angle += tempo;
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

            Vector3 modelForwardVector = SceneManager.GetModelQuaterionVectors(c_models[i].model_id).Forward;
            Vector3 modelUpVector = SceneManager.GetModelQuaterionVectors(c_models[i].model_id).Up;

            std::tuple rotationInfo = GetRotationTowardsPoint(modelForwardVector, modelUpVector, 
                                                            c_transforms[i].Position, c_goals[i].PathToGoal[c_goals[i].steps]);

            Vector3 rotationAxis = std::get<0>(rotationInfo);
            float rotationAngle = std::get<1>(rotationInfo);;

            c_transforms[i].TargetAngle = rotationAngle;

            c_transforms[i].AngleGrowth = abs(c_transforms[i].RotationAngle - rotationAngle) / 10;

            if (rotationAngle < c_transforms[i].RotationAngle)
                c_transforms[i].AngleGrowth = -c_transforms[i].AngleGrowth;

            c_transforms[i].RotationAngle += c_transforms[i].AngleGrowth;

            if (c_transforms[i].AngleGrowth > 0)
            {
                if (c_transforms[i].RotationAngle > c_transforms[i].TargetAngle)
                {
                    c_transforms[i].RotationAngle = c_transforms[i].TargetAngle;
                    c_transforms[i].AngleGrowth = 0.0f;
                }
            }
            else if (c_transforms[i].AngleGrowth < 0)
            {
                if (c_transforms[i].RotationAngle < c_transforms[i].TargetAngle)
                {
                    c_transforms[i].RotationAngle = c_transforms[i].TargetAngle;
                    c_transforms[i].AngleGrowth = 0.0f;
                }
            }
            
            c_transforms[i].RotationAxis = {0, 1, 0};

            Vector3 scaleVector = {c_models[i].scale, c_models[i].scale, c_models[i].scale};
            DrawModelEx(currentModel, c_transforms[i].Position, c_transforms[i].RotationAxis, 
                                                                c_transforms[i].RotationAngle, scaleVector, WHITE);

            c_transforms[i].RotationSwitchFrameDelay++;

            if (m_UnitFlags.DrawDebugPath)
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