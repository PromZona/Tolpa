#pragma once
#include <memory>
#include <vector>
#include "raylib.h"

#include "ECS/ECS.hpp"
#include "renderer.hpp"
#include "InputManager.hpp"
#include "Controllers/IController.hpp"
#include "GameState.h"
#include "GuiManager.hpp"
#include "Commands/CommandManager.hpp"

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
	GUIManager& GetGUI();
	CommandManager& GetCommandManager();
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
	ECS m_ECS;
	GUIManager m_guiManager;
	CommandManager m_CommandManager;

	std::vector<std::unique_ptr<IController>> m_controllers;
};