#pragma once
#include <memory>
#include <vector>
#include "raylib.h"

#include "renderer.hpp"
#include "InputManager.hpp"
#include "Controllers/IController.hpp"

class EntityManager;

class Game
{
public:
    Game();
    ~Game();
    void Start();
private:
    void Input();
    void Update();
    void Render();
    void InitializeControllers();

    Renderer m_renderer;
    InputManager m_inputManager;
    EntityManager& m_entityManager;

    std::vector<std::unique_ptr<IController>> m_controllers;
};