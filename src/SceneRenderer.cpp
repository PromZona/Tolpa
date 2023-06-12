#include "../include/SceneRenderer.hpp"

#include "Components/RenderComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/ModelComponent.hpp"

#include "game.hpp"
#include "rlgl.h"

SceneRenderer::SceneRenderer(){}

SceneRenderer::~SceneRenderer()
{
    UnloadShader(m_shader_light);
}

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

    m_light = CreateLight(LIGHT_POINT, {0, 150, 0}, Vector3Zero(), BLUE, m_shader_light);
}

void SceneRenderer::ApplyLightingShaderToObjects()
{
    auto& sceneManager = Game::Instance().GetSceneManager();

    sceneManager.GetModel(ModelType::MAP).materials[0].shader = m_shader_light;
}

Light CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader)
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

void UpdateLightValues(Shader shader, Light light)
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

    BeginDrawing();
    
    ClearBackground(BLACK);
    DrawFPS(5, 5);
    
    BeginMode3D(m_camera);

    for (auto& archetype : archetypes)
    {
        auto& transforms = archetype->GetComponents<TransformComponent>();
        auto& renders = archetype->GetComponents<RenderComponent>();
        auto& models = archetype->GetComponents<ModelComponent>();

        std::size_t size = transforms.size();

        for (std::size_t i = 0; i < size; i++)
        {
            DrawModel(SceneManager.GetModel(models[i].model_id), transforms[i].Position, models[i].scale, WHITE);
        }
    }

    auto& middleMesh = NavGrid.GetTriangles();

    for (int i = 0; i < middleMesh.size(); i++)
        DrawPoint3D(middleMesh[i].middlePoint, GREEN);

    auto& navGridModel = NavGrid.GetModel();

    NavGrid.DebugDrawGrid();
    NavGrid.DebugDrawConnectedTriangles();

    DrawSphereEx(m_light.position, 0.5f, 8, 8, m_light.color);

    EndMode3D();

    Game::Instance().GetGUI().DrawGUI();

    DrawText(TextFormat("Camera Pos: %3.2f %3.2f %3.2f",
                        m_camera.position.x,
                        m_camera.position.y,
                        m_camera.position.z), 10, 70 + 45, 10, WHITE);
    
    EndDrawing();
}