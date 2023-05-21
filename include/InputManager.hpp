#pragma once

class EntityManager;

class InputManager{
public:
    InputManager();
    ~InputManager();

    void Update();
private:
    EntityManager& m_entityManager;
};