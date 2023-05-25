#include "game.hpp"

#include "EntityManager.hpp"
#include "Entity.hpp"

#include "Controllers/MovementController.hpp"
#include "Controllers/UnitGoalController.hpp"

Game& Game::m_instance = Game::Instance();

Game::Game() : m_sceneManager(), m_sceneRenderer(), m_inputManager(), m_entityManager(EntityManager::Instance()), m_controllers()
{
    const int screenWidth = 1280;
    const int screenHeight = 1024;
    InitWindow(screenWidth, screenHeight, "Tolpa");
    SetTargetFPS(60);
}

Game::~Game()
{
    CloseWindow();
}

void Game::Start()
{
    InitializeControllers();
    InitializeScene();

    while (!WindowShouldClose()) {
        Input();
        Update();
        Render();
    }
}

void Game::Input()
{
    m_inputManager.Update();
}

void Game::Update()
{
    float deltaTime = GetFrameTime();
    for (auto& controller : m_controllers)
    {
        controller->Tick(deltaTime);
    }
}

void Game::Render()
{
    m_sceneRenderer.RenderScene(&m_sceneManager);
}

void Game::InitializeControllers()
{
    m_controllers.emplace_back(std::make_unique<MovementController>());
    m_controllers.emplace_back(std::make_unique<UnitGoalController>());
}

void Game::InitializeScene()
{
    m_sceneRenderer.InitializeCamera();
    
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    //TODO: Move Somewhere else ---------------------------------------------------------------------------------
    if (IsShaderReady(m_sceneRenderer.m_shader_light))
        printf("shader ready\n");
    else
        printf("shader not ready\n");

    m_sceneRenderer.m_shader_light = LoadShader("../resources/shaders/lighting.vs",
                                                "../resources/shaders/lighting.fs");

    m_sceneRenderer.m_shader_light.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(m_sceneRenderer.m_shader_light, "viewPos");
    //m_sceneRenderer.m_shader_light.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(m_sceneRenderer.m_shader_light, "matModel");

    m_sceneRenderer.m_ambient_loc = GetShaderLocation(m_sceneRenderer.m_shader_light, "ambient");
    
    float temp[4] = {0.1f, 0.1f, 0.1f, 1.0f}; // Function below doesnt eat these raw values for some reason
    SetShaderValue(m_sceneRenderer.m_shader_light, m_sceneRenderer.m_ambient_loc, temp, SHADER_UNIFORM_VEC4);

    m_sceneRenderer.m_light = CreateLight(LIGHT_POINT, {0, 150, 0}, Vector3Zero(), WHITE, m_sceneRenderer.m_shader_light);
    //TODO: Move Somewhere else ---------------------------------------------------------------------------------

    m_sceneManager.AddObjectToScene(new GameObject("../resources/3d_objects/landscape.glb", 
                                                   "../resources/3d_objects/landscape.png",
                                                   TERRAIN, {0.0f, 0.0f, 0.0f}, "Land"));

    m_sceneRenderer.ApplyLightingShaderToObjects(&m_sceneManager);
}

std::vector<int> &Game::GetUnits()
{
    return m_instance.m_units;
}

std::vector<int> &Game::GetCities()
{
    return m_instance.m_cities;
}

