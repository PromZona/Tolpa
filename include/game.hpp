#pragma once
#include <memory>
#include "raylib.h"

#include "renderer.hpp"
#include "InputManager.hpp"
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

    Renderer m_renderer;
    InputManager m_inputManager;
    EntityManager& m_entityManager;
};