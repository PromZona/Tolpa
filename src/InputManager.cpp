#include "InputManager.hpp"

#include <raylib.h>
#include "EntityFactory.hpp"
#include "game.hpp"

#include "Components/TransformComponent.hpp"

InputManager::InputManager()
{
}
InputManager::~InputManager() = default;

void InputManager::Update()
{
	static float offset = 0.0f;
	static bool first = true;

	int keyPressed = 0;
	while ((keyPressed = GetKeyPressed()) != 0)
	{
		if (keyPressed == KEY_B)
		{
			auto& lairs = Game::Instance().State.lairs;
			if (lairs.empty())
				continue;
			auto unit = EntityFactory::CreateEnemy("../resources/3d_objects/enemy.glb");

			auto& ecs = Game::Instance().GetECS();
			auto lairPos = ecs.GetComponent<TransformComponent>(lairs[GetRandomValue(0, lairs.size() - 1)])->Position;
			ecs.GetComponent<TransformComponent>(unit)->Position = lairPos;
			offset += 2;
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

			auto unit = EntityFactory::CreateParty("../resources/3d_objects/party.glb");
			auto& ecs = Game::Instance().GetECS();
			ecs.GetComponent<TransformComponent>(unit)->Position = mousePos;
			offset += 2;
			continue;
		}
    
		if (keyPressed == KEY_C)
		{
			Vector3 mousePos; //GetMousePosition();

			if (first)
				mousePos = {0, 0, offset};
			else
				mousePos = {offset, 5.0f, offset};

			auto city = EntityFactory::CreateCity("../resources/3d_objects/city.glb");
			auto& ecs = Game::Instance().GetECS();
			auto transform= ecs.GetComponent<TransformComponent>(city);
			transform->Position = mousePos;

			offset += 20;
			first = !first;

			continue;
		}
		if (keyPressed == KEY_V)
		{
			Vector3 mousePos = {0, offset, 0}; //GetMousePosition();
			auto lair = EntityFactory::CreateLair("../resources/3d_objects/lair.glb");
			auto& ecs = Game::Instance().GetECS();
			ecs.GetComponent<TransformComponent>(lair)->Position = mousePos;
			offset += 2;
			continue;
		}

		if (keyPressed == KEY_Z)
		{
			Game::Instance().GetECS().PrintInfo();
		}
	}
}