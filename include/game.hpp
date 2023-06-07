#pragma once
#include <memory>
#include <vector>
#include "raylib.h"

#include "ECS/ECS.hpp"
#include "SceneRenderer.hpp"
#include "InputManager.hpp"
#include "Controllers/IController.hpp"
#include "GameState.h"

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

	ECS& GetECS();
	GameState State;
private:
	Game();
	static Game& m_instance;

	void Input();
	void Update();
	void Render();
	void InitializeControllers();
	void InitializeScene();

	SceneRenderer m_renderer;
	InputManager m_inputManager;
	ECS m_ECS;

	std::vector<std::unique_ptr<IController>> m_controllers;
};