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

<<<<<<< Updated upstream
    static std::vector<int>& GetUnits();
    static std::vector<int>& GetCities();
=======
	GameState State;
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
    std::vector<std::unique_ptr<IController>> m_controllers;

    std::vector<int> m_units;
    std::vector<int> m_cities;
=======
	std::vector<std::unique_ptr<IController>> m_controllers;
>>>>>>> Stashed changes
};