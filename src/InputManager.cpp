#include "InputManager.hpp"

#include <raylib.h>
#include "EntityFactory.hpp"
#include "game.hpp"

#include "Components/TransformComponent.hpp"

// --------------------------
// ---- DELETE LATER --------
#include <random>

Vector3 GetRandomLocation()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    float min = -100.0f;
    float max = 100.0f;

    std::uniform_real_distribution<float> dis(min, max);

    return {dis(gen), dis(gen), dis(gen)};
}
//--------------------------
//--------------------------

InputManager::InputManager()
{
}
InputManager::~InputManager() = default;

void InputManager::Update()
{
	int keyPressed = 0;
	while ((keyPressed = GetKeyPressed()) != 0)
	{
		if (keyPressed == KEY_B)
		{
			auto& lairs = Game::Instance().State.lairs;
			if (lairs.empty())
				continue;
			auto unit = EntityFactory::CreateEnemy();

			auto& ecs = Game::Instance().GetECS();
			auto lairPos = ecs.GetComponent<TransformComponent>(lairs[GetRandomValue(0, lairs.size() - 1)])->Position;
			ecs.GetComponent<TransformComponent>(unit)->Position = lairPos;
			continue;
		}

		if (keyPressed == KEY_N)
		{

			// -----------------------------------------------------------
			// ------------- NEEDS IMPLEMENTATION FOR 3D -----------------
			// -----------------------------------------------------------
			Vector3 mousePos = {0, 0, 0}; //GetMousePosition();
			// -----------------------------------------------------------
			// -----------------------------------------------------------

			auto unit = EntityFactory::CreateParty();
			auto& ecs = Game::Instance().GetECS();
			ecs.GetComponent<TransformComponent>(unit)->Position = mousePos;
			continue;
		}
    
		if (keyPressed == KEY_C)
		{
			Vector3 mousePos; //GetMousePosition();

			auto city = EntityFactory::CreateCity();
			auto& ecs = Game::Instance().GetECS();
			auto transform= ecs.GetComponent<TransformComponent>(city);
			transform->Position = GetRandomLocation();
			continue;
		}
		if (keyPressed == KEY_V)
		{
			Vector3 mousePos = {0, 0, 0}; //GetMousePosition();
			auto lair = EntityFactory::CreateLair();
			auto& ecs = Game::Instance().GetECS();
			ecs.GetComponent<TransformComponent>(lair)->Position = mousePos;
			continue;
		}

		if (keyPressed == KEY_Z)
		{
			Game::Instance().GetECS().PrintInfo();
		}
	}
}