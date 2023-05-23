#include "game.hpp"

#include "EntityManager.hpp"
#include "Entity.hpp"

#include "Controllers/MovementController.hpp"
#include "Controllers/UnitGoalController.hpp"

Game& Game::m_instance = Game::Instance();

Game::Game() : m_renderer(), m_inputManager(), m_entityManager(EntityManager::Instance()), m_controllers()
{
	const int screenWidth = 800;
	const int screenHeight = 450;
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
	m_renderer.Render();
}

void Game::InitializeControllers()
{
	m_controllers.emplace_back(std::make_unique<MovementController>());
	m_controllers.emplace_back(std::make_unique<UnitGoalController>());
}

std::vector<int>& Game::GetUnits()
{
	return m_instance.m_units;
}

std::vector<int>& Game::GetCities()
{
	return m_instance.m_cities;
}

