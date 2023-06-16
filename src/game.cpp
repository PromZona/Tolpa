#include "game.hpp"

#include "Controllers/MovementController.hpp"
#include "Controllers/UnitGoalController.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/GoalComponent.hpp"
#include "Components/ModelComponent.hpp"
#include "Components/LocationComponent.hpp"
#include "Components/TerrainComponent.hpp"

Game& Game::m_instance = Game::Instance();

Game::Game() :
	m_rendererScene(), 
	m_rendererLocations(), 
	m_rendererUnits(),
	m_inputManager(),
	m_controllers(),
	m_ECS(),
	m_guiManager(),
	m_CommandManager()
{
	InitWindow(ScreenWidth, ScreenHeight, "Tolpa");
	SetTargetFPS(60);
	m_guiManager.Init();
}

Game::~Game()
{
	CloseWindow();
}

void Game::Start()
{
    InitializeControllers();
	InitializeRenderers();
	InitializeScene();

	while (!WindowShouldClose())
	{
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
	m_CommandManager.Tick();

	float deltaTime = GetFrameTime();
	for (auto& controller : m_controllers)
	{
		controller->Tick(deltaTime);
	}
}

void Game::Render()
{
	BeginDrawing();
	ClearBackground(BLACK);
	DrawFPS(5, 5);

	BeginMode3D(m_rendererScene.GetCamera());

	m_rendererScene.RenderScene();
	m_rendererUnits.RenderUnits();
	m_rendererLocations.RenderLocations();

	EndMode3D();
	
	m_rendererGUI.RenderGUI();
	
	EndDrawing();
}

void Game::InitializeRenderers()
{
	m_ECS.RegisterComponentInSystem<TransformComponent>(m_rendererScene);
	m_ECS.RegisterComponentInSystem<RenderComponent>(m_rendererScene);
	m_ECS.RegisterComponentInSystem<ModelComponent>(m_rendererScene);
	m_ECS.RegisterComponentInSystem<TerrainComponent>(m_rendererScene);

	m_ECS.RegisterComponentInSystem<TransformComponent>(m_rendererUnits);
	m_ECS.RegisterComponentInSystem<RenderComponent>(m_rendererUnits);
	m_ECS.RegisterComponentInSystem<ModelComponent>(m_rendererUnits);
	m_ECS.RegisterComponentInSystem<MovementComponent>(m_rendererUnits);
	m_ECS.RegisterComponentInSystem<GoalComponent>(m_rendererUnits);

	m_ECS.RegisterComponentInSystem<TransformComponent>(m_rendererLocations);
	m_ECS.RegisterComponentInSystem<RenderComponent>(m_rendererLocations);
	m_ECS.RegisterComponentInSystem<ModelComponent>(m_rendererLocations);
	m_ECS.RegisterComponentInSystem<LocationComponent>(m_rendererLocations);
}

void Game::InitializeControllers()
{
	MovementController movementController;
	m_ECS.RegisterComponentInSystem<TransformComponent>(movementController);
	m_ECS.RegisterComponentInSystem<MovementComponent>(movementController);
	m_controllers.emplace_back(std::make_unique<MovementController>(movementController));


	UnitGoalController unitGoalController;
	m_ECS.RegisterComponentInSystem<TransformComponent>(unitGoalController);
	m_ECS.RegisterComponentInSystem<MovementComponent>(unitGoalController);
	m_ECS.RegisterComponentInSystem<GoalComponent>(unitGoalController);
	m_controllers.emplace_back(std::make_unique<UnitGoalController>(unitGoalController));
}

void Game::InitializeScene()
{
    m_sceneManager.InitializeScene();
}

ECS& Game::GetECS()
{
	return m_ECS;
}

GUIManager& Game::GetGUI()
{
	return m_guiManager;
}

SceneManager& Game::GetSceneManager()
{
	return m_sceneManager;
}

NavMesh& Game::GetNavGrid()
{
	return m_navGrid;
}
CommandManager& Game::GetCommandManager()
{
	return m_CommandManager;
}
