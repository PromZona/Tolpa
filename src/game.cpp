#include "game.hpp"

#include "EntityManager.hpp"
#include "Entity.hpp"

#include "Controllers/MovementController.hpp"
#include "Controllers/UnitGoalController.hpp"

Game& Game::m_instance = Game::Instance();

Game::Game() : m_scene_renderer(), m_inputManager(), m_entityManager(EntityManager::Instance()), m_controllers()
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

    m_scene_renderer.ClearScene();
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
    m_scene_renderer.RenderScene();
}

void Game::InitializeControllers()
{
    m_controllers.emplace_back(std::make_unique<MovementController>());
    m_controllers.emplace_back(std::make_unique<UnitGoalController>());
}

void Game::InitializeScene()
{
    m_scene_renderer.InitializeCamera();
    
    m_scene_renderer.RenderModel("../resources/3d_objects/landscape.glb", "../resources/3d_objects/landscape.png", {0.0f, 0.0f, 0.0f}, std::string("Land"));
    m_scene_renderer.RenderModel("../resources/3d_objects/water.glb", "../resources/3d_objects/r.png", {0.0f, 1.9f / 2 * m_scene_renderer.m_model_render_scale, 0.0f}, std::string("Ocean"));
}

std::vector<int> &Game::GetUnits()
{
    return m_instance.m_units;
}

std::vector<int> &Game::GetCities()
{
    return m_instance.m_cities;
}

