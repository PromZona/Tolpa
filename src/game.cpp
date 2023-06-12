#include "game.hpp"

#include "Controllers/MovementController.hpp"
#include "Controllers/UnitGoalController.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/GoalComponent.hpp"
#include "Components/ModelComponent.hpp"

Game& Game::m_instance = Game::Instance();

Game::Game() : m_renderer(), m_inputManager(), m_controllers(), m_ECS(), m_guiManager()
{	
	InitWindow(screenWidth, screenHeight, "Tolpa");
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
	InitializeScene();
	m_ECS.RegisterComponentInSystem<TransformComponent>(m_renderer);
	m_ECS.RegisterComponentInSystem<RenderComponent>(m_renderer);
	m_ECS.RegisterComponentInSystem<ModelComponent>(m_renderer);

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
	float deltaTime = GetFrameTime();
	for (auto& controller : m_controllers)
	{
		controller->Tick(deltaTime);
	}
}

void Game::Render()
{
    m_renderer.RenderScene();
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

#include <random>
void Game::DebugTestTestDebugDeleteLater()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	int min = 0;
	int max = Game::Instance().GetNavGrid().GetTriangles().size();
    std::uniform_int_distribution<int> dis(min, max);

	auto& ecs = Game::Instance().GetECS();

	for (int i = 0; i < 10; i++)
	{
		auto city = ecs.CreateEntity();

		ecs.AddComponent<TransformComponent>(city, 
		{Game::Instance().GetNavGrid().GetTriangles()[dis(gen)].middlePoint});
		ecs.AddComponent<ModelComponent>(city, {ModelType::CITY, 1.0f});
		ecs.AddComponent<RenderComponent>(city, {RED, 8.0f});

		Game::Instance().State.cities.push_back(city);
	}

	for (int i = 0; i < 200; i++)
	{
		auto unit = ecs.CreateEntity();
		ecs.AddComponent<TransformComponent>(unit, 
		{Game::Instance().GetNavGrid().GetTriangles()[dis(gen)].middlePoint});

		ecs.AddComponent<ModelComponent>(unit, {ModelType::PARTY, 0.1f});
		ecs.AddComponent<RenderComponent>(unit, {RED, 8.0f});
		ecs.AddComponent<MovementComponent>(unit, {{0, 0, 0}, 20.0f});
		ecs.AddComponent<GoalComponent>(unit, {
		Game::Instance().GetNavGrid().GetTriangles()[dis(gen)].middlePoint, {}, false, 0});

		Game::Instance().State.parties.push_back(unit);
	}
}

void Game::InitializeScene()
{
    m_renderer.InitializeCamera();
	m_sceneManager.LoadModels();
	
	auto& ecs = Game::Instance().GetECS();
	auto map = ecs.CreateEntity();

	ecs.AddComponent<TransformComponent>(map, {{0, 0, 0}});
	ecs.AddComponent<ModelComponent>(map, {ModelType::MAP, 1.0f});
	ecs.AddComponent<RenderComponent>(map, {RED, 8.0f});

	Game::Instance().State.map.push_back(map);

	m_renderer.InitializeLighting();
	m_renderer.ApplyLightingShaderToObjects();

	m_navGrid.CalculateMiddlePoints();
	m_navGrid.ConstructMeshGraph();

	DebugTestTestDebugDeleteLater();
}

ECS& Game::GetECS()
{
	return m_ECS;
}

SceneRenderer& Game::GetSceneRenderer()
{
	return m_renderer;
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