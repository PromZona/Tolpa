#pragma once
#include <memory>
#include <vector>
#include "raylib.h"

#include "ECS/ECS.hpp"
#include "Renderer.hpp"
#include "SceneManager.hpp"
#include "InputManager.hpp"
#include "NavigationMesh.hpp"
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
	
	inline SceneRenderer& GetRendererScene() {return m_rendererScene;}
	inline UnitRenderer& GetRendererUnits() {return m_rendererUnits;}
	inline LocationRenderer& GetRendererLocations() {return m_rendererLocations;}
	inline GuiRenderer& GetRendererGUI() {return m_rendererGUI;}

	SceneManager& GetSceneManager();
	NavMesh& GetNavGrid();
	GUIManager& GetGUI();
	CommandManager& GetCommandManager();
	GameState State;

	const int ScreenWidth = 1600;
	const int ScreenHeight = 1400;

private:
	Game();
	static Game& m_instance;

	void Input();
	void Update();
	void Render();
	void InitializeControllers();
	void InitializeRenderers();
	void InitializeScene();

	NavMesh m_navGrid;

	SceneRenderer m_rendererScene;
	UnitRenderer m_rendererUnits;
	LocationRenderer m_rendererLocations;
	GuiRenderer m_rendererGUI;

	SceneManager m_sceneManager;
	InputManager m_inputManager;
	ECS m_ECS;
	GUIManager m_guiManager;
	CommandManager m_CommandManager;

	std::vector<std::unique_ptr<IController>> m_controllers;
};