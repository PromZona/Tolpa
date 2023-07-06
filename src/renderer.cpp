#include "../include/Renderer.hpp"

#include "Components/RenderComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/ModelComponent.hpp"
#include "Components/GoalComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/LocationComponent.hpp"
#include "Components/TerrainComponent.hpp"
#include "Components/RotationComponent.hpp"

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

// ------------------------------------------------------------------------
// ----------- SUPPORT FUNCTIONS DECLARATION ------------------------------

void UnitRotationTick(RotationComponent& unitRotation);
float GetRotationTowardsPoint(Vector3 forward, Vector3 position, Vector3 destination);
float GetAngleDiff(float current, float target);

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------

// @note Initial Camera settings
void SceneRenderer::InitializeCamera()
{
    TraceLog(LOG_INFO, "Initializing Camera...");
    m_camera = { 0 };
    m_camera.position = {200.0f, 200.0f, 200.0f};
    m_camera.target = {0.0f, 0.0f, 0.0f};
    m_camera.up = {0.0f, 1.0f, 0.0f};
    m_camera.fovy = 90.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

// @note 1) Loading vertex/fragment shaders from files
// @note 2) Setting their initial values and ConfigFlags
// @note 3) Creating ambient light point on the scene
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

// @note Applying ambient lighting shaders to all 3d models loaded in Scene Manager
// @warning Currently applies shader to ALL models (may not be desired later) 
void SceneRenderer::ApplyLightingShaderToObjects()
{
    TraceLog(LOG_INFO, "Applying shader to scene models...");

    auto& sceneManager = Game::Instance().GetSceneManager();

    auto& model_map = sceneManager.GetTypeToModelMap();

    // Cycling through the whole map and applying shader to every entry
    // Meaning EVERY 3d model will be influenced by the lighting point
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

// @note Simple Debug function to demonstrate that light point is actually working.
// @note It just rotates the light point around the scene.
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

// @note Renders wireframe of the object
// @note Used when placing anything into the scene with a mouse
void SceneRenderer::RenderPlacingPreview()
{
    auto& hitPoint = m_meshPicker.GetCollisionPoint();
    auto& sceneManager = Game::Instance().GetSceneManager();

    float previewScale = 1.0f;
    switch (m_gameplayVariables.previewModelType)
    {
        case ModelType::HUMAN:
            // This should be 1.0f but current human model is way too big so fix later?
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

// @note 1) Scene-specific Renders
// @note 2) Camera updates
// @note 3) Shader updates
// @note 4) Terrain rendering
// @note 5) Gameplay information rendering
// @note 6) Debug information rendering
void SceneRenderer::RenderScene()
{
    auto& ecs = Game::Instance().GetECS();
    auto& SceneManager = Game::Instance().GetSceneManager();
    auto& NavGrid = Game::Instance().GetNavGrid();
	auto archetypes = ecs.GetRequiredArchetypes(Archetype);
    
    // ------------------------------------------------------------------------
    // ----------------- RENDERING SHADERS ------------------------------------

    float cameraPos[3] = {m_camera.position.x, m_camera.position.y, m_camera.position.z};
    SetShaderValue(m_shader_light, m_shader_light.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

    if (IsKeyPressed(KEY_B))
        m_light.enabled = !m_light.enabled;

    UpdateLightValues(m_shader_light, m_light);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------

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

    // ------------------------------------------------------------------------
    // --------------- RENDERING GAMEPLAY-NEEDED INFORMATION ------------------

    if (m_gameplayVariables.ShowPlacingPreview)
    {
        m_meshPicker.GetTerrainHit();
        RenderPlacingPreview();
    }

    // ------------------------------------------------------------------------
    // -------------- RENDERING VARIOUS OPTIONAL INFORMATION ------------------

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

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------

    // Draws a small sphere where the light point is
    DrawSphereEx(m_light.position, 0.5f, 8, 8, m_light.color);
}

// @param [forward] Forward vector of unit's 3d model
// @param [position] Unit position from transform component
// @param [destination] What point this unit should be facing
// @returns Rotation angle needed to face the [destination] point
float GetRotationTowardsPoint(Vector3 forward, Vector3 position, Vector3 destination)
{
    Vector3 desiredForward = Vector3Normalize(Vector3Subtract(destination, position));

    Vector3 rotationAxis = Vector3CrossProduct(forward, desiredForward);
    rotationAxis = Vector3Normalize(rotationAxis);

    float angle = acos(Vector3DotProduct(forward, desiredForward));
    float angleDegrees = angle * (180.0f / PI);

    if (rotationAxis.y < 0)
        angleDegrees = -angleDegrees;

    return angleDegrees;        
}

// @param [current] current unit rotation angle
// @param [target] target unit rotation angle
// @param [diff] difference between these angles
float GetAngleDiff(float current, float target)
{
    float diff = fmodf((current - target + 180), 360.0f) - 180;
    return diff < -180 ? -(diff + 360) : -diff; 
}

// @note Called every frame to smoothly update unit's angle 
// @note Smoothness depends on RotationComponents parameters
// @param [unitRotation] RotationComponent of rotating entity
void UnitRotationTick(RotationComponent& unitRotation)
{
    // Should be turned into a conditional (TODO?)
    unitRotation.AngleGrowth = GetAngleDiff(unitRotation.CurrentAngle, unitRotation.TargetAngle) / unitRotation.FramesToRotate;

    // Smooth rotation
    unitRotation.CurrentAngle += unitRotation.AngleGrowth;

    // Block rotation beyond target (will spin infinitely otherwise)
    if (unitRotation.AngleGrowth < 0)
    {
        if (unitRotation.CurrentAngle < unitRotation.TargetAngle)
            unitRotation.CurrentAngle = unitRotation.TargetAngle;
    }
    else
        if (unitRotation.CurrentAngle > unitRotation.TargetAngle)
            unitRotation.CurrentAngle = unitRotation.TargetAngle;
    
    // Rotation axis is always Y only, otherwise units go crazy on steep surfaces
    unitRotation.RotationAxis = {0, 1, 0};
}

// @note 1) EntityUnit-Specific Renders
// @note 2) Unit model rendering
// @note 3) Path drawing
// @note 4) Model Rotation
void UnitRenderer::RenderUnits()
{
    auto& ecs = Game::Instance().GetECS();
    auto& SceneManager = Game::Instance().GetSceneManager();
    auto& NavGrid = Game::Instance().GetNavGrid();
	auto archetypes = ecs.GetRequiredArchetypes(Archetype);

    for (auto& archetype : archetypes)
    {
        auto& c_transforms = archetype->GetComponents<TransformComponent>();
        auto& c_rotation = archetype->GetComponents<RotationComponent>();
        auto& c_renders = archetype->GetComponents<RenderComponent>();
        auto& c_models = archetype->GetComponents<ModelComponent>();
        auto& c_goals = archetype->GetComponents<GoalComponent>();
        auto& c_movement = archetype->GetComponents<MovementComponent>();

        std::size_t size = c_transforms.size();

        for (std::size_t i = 0; i < size; i++)
        {
            // --------------------------------------------------------------------------
            // ---------------------- This could? be further optimised later ------------
            RotationComponent& unitRotation = c_rotation[i];
            Vector3 modelForwardVector = SceneManager.GetModelQuaterionVectors(c_models[i].model_id).Forward;

            unitRotation.TargetAngle = GetRotationTowardsPoint(modelForwardVector, c_transforms[i].Position, c_goals[i].PathToGoal[c_goals[i].steps]);

            UnitRotationTick(unitRotation);
            // --------------------------------------------------------------------------
            // --------------------------------------------------------------------------

            Model currentModel = SceneManager.GetModel(c_models[i].model_id);
            Vector3 scaleVector = {c_models[i].scale, c_models[i].scale, c_models[i].scale};
            DrawModelEx(currentModel, c_transforms[i].Position, unitRotation.RotationAxis, 
                                                                unitRotation.CurrentAngle, scaleVector, WHITE);

            if (m_UnitFlags.DrawDebugPath)
                NavGrid.DebugDrawPath(c_goals[i].PathToGoal, c_goals[i].steps);
        }
    }
}

// @note Renders every location unit (city, tribe etc.)
// @note 1) Location-Specific Renders
// @note 2) Location model rendering
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

// @warning This way is essential
// @note Renders everything that needs to be rendered in 2D
// @note Meaning outside of Begin3DMode/End3DMode 
// @note 1) User Interface
// @note 2) Coming soon?
void GuiRenderer::RenderGUI()
{
    Game::Instance().GetGUI().DrawGUI();

    DrawText("Controls:", 10, 35, 20, DARKGRAY);
    DrawText("[C] - Create City", 10, 60, 20, DARKGRAY);
    DrawText("[H] - Create Human", 10, 85, 20, DARKGRAY);
    DrawText("[T] - Create Tribe", 10, 110, 20, DARKGRAY);
    DrawText("[O] - Create Orc", 10, 135, 20, DARKGRAY);
}