#pragma once
#include <memory>
#include <vector>
#include "raylib.h"

#include "renderer.hpp"
#include "InputManager.hpp"
#include "Controllers/IController.hpp"
#include "Entity.hpp"
#include "GameState.h"

class EntityManager;

class Game
{
public:
	static Game& Instance()
	{
		static Game m_instance;
		return m_instance;
	}

	~Game();
	void Start();

	GameState State;
private:
	Game();
	static Game& m_instance;

	void Input();
	void Update();
	void Render();
	void InitializeControllers();

	Renderer m_renderer;
	InputManager m_inputManager;
	EntityManager& m_entityManager;

	std::vector<std::unique_ptr<IController>> m_controllers;
};