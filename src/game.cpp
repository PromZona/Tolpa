#include "game.hpp"

#include "Controllers/MovementController.hpp"
#include "Controllers/UnitGoalController.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/GoalComponent.hpp"
#include "Components/ModelComponent.hpp"

Game& Game::m_instance = Game::Instance();

Game::Game() : m_renderer(), m_inputManager(), m_controllers(), m_ECS()
{
	const int screenWidth = 1200;
	const int screenHeight = 1200;
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

void Game::InitializeScene()
{
    m_renderer.InitializeCamera();
}

ECS& Game::GetECS()
{
	return m_ECS;
}
