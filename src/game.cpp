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
    
    m_sceneManager.AddObjectToScene(new GameObject("../resources/3d_objects/landscape.glb", 
                                                   "../resources/3d_objects/landscape.png",
                                                   TERRAIN, {0.0f, 0.0f, 0.0f}, "Land"));

    m_sceneManager.AddObjectToScene(new GameObject("../resources/3d_objects/water.glb", 
                                                   "../resources/3d_objects/water.png",
                                                   TERRAIN, {0.0f, 0.95f, 0.0f}, "Water"));
}

std::vector<int> &Game::GetUnits()
{
    return m_instance.m_units;
}

std::vector<int> &Game::GetCities()
{
    return m_instance.m_cities;
}

