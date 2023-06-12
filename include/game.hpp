#pragma once
#include <memory>
#include <vector>
#include "raylib.h"

#include "ECS/ECS.hpp"
#include "SceneRenderer.hpp"
#include "SceneManager.hpp"
#include "InputManager.hpp"
#include "NavigationMesh.hpp"
#include "Controllers/IController.hpp"
#include "GameState.h"
#include "GuiManager.hpp"

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

	void DebugTestTestDebugDeleteLater();

	ECS& GetECS();
	SceneRenderer& GetSceneRenderer();
	SceneManager& GetSceneManager();
	NavMesh& GetNavGrid();
	GUIManager& GetGUI();
	GameState State;

	const int screenWidth = 1600;
	const int screenHeight = 1400;

private:
	Game();
	static Game& m_instance;

	void Input();
	void Update();
	void Render();
	void InitializeControllers();
	void InitializeScene();

	NavMesh m_navGrid;

	SceneRenderer m_renderer;
	SceneManager m_sceneManager;
	InputManager m_inputManager;
	ECS m_ECS;
	GUIManager m_guiManager;

	std::vector<std::unique_ptr<IController>> m_controllers;
};