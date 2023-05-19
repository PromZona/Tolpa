#pragma once
#include <memory>
#include "raylib.h"

#include "renderer.hpp"
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
    EntityManager& m_entityManager;
};