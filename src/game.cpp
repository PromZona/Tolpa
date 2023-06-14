#include "game.hpp"

#include <string>
#include "Logger/Logger.hpp"

#include "Controllers/MovementController.hpp"
#include "Controllers/UnitGoalController.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/GoalComponent.hpp"

Game& Game::m_instance = Game::Instance();

Game::Game() :
	m_renderer(),
	m_inputManager(),
	m_controllers(),
	m_ECS(),
	m_guiManager(),
	m_CommandManager()
{
	m_guiManager.Init();
}

Game::~Game()
{
	CloseWindow();
}

void Game::Start()
{
	SetTraceLogCallback(Logger::RaylibLoggingRedirect);

	m_ECS.RegisterComponentInSystem<TransformComponent>(m_renderer);
	m_ECS.RegisterComponentInSystem<RenderComponent>(m_renderer);
	InitializeControllers();

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
	m_renderer.Render();
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

ECS& Game::GetECS()
{
	return m_ECS;
}

GUIManager& Game::GetGUI()
{
	return m_guiManager;
}

CommandManager& Game::GetCommandManager()
{
	return m_CommandManager;
}
