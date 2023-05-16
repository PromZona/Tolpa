#pragma once
#include "raylib.h"

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
};