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
	auto& cam = Game::Instance().GetSceneRenderer().GetCamera();
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

	Vector2 prevMousePosition = {0.0f, 0.0f};
	Vector2 currPosition = {0.0f, 0.0f};
	Vector2 mouseDelta = {0.0f, 0.0f};

	Vector3 camPosition;
	float cameraSpeed = 0.01f;

	if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_RIGHT))
	{
		prevMousePosition = GetMousePosition();
		
		BeginDrawing();

		DrawText(TextFormat("Current Mouse Pos: %3.2f %3.2f",
                        currPosition.x,
                        currPosition.y), 10, 70 + 30, 10, WHITE);

		DrawText(TextFormat("Prev Mouse Pos: %3.2f %3.2f",
                        prevMousePosition.x,
                        prevMousePosition.y), 10, 70 + 15, 10, WHITE);

		EndDrawing();
	}

	if (IsMouseButtonDown(MouseButton::MOUSE_BUTTON_RIGHT))
	{
		currPosition = {GetMousePosition().x - Game::Instance().screenWidth / 2,
						GetMousePosition().y - Game::Instance().screenHeight / 2};

		mouseDelta = {currPosition.x - prevMousePosition.x, currPosition.y - prevMousePosition.y};

		mouseDelta = Vector2Normalize(mouseDelta);

		cam.position = Vector3Subtract(cam.position, cam.target);
		cam.position = Vector3RotateByAxisAngle(cam.position, {0.0f, 1.0f, 0.0f}, -mouseDelta.x * cameraSpeed);

		prevMousePosition = currPosition;
	}

	float wheel = GetMouseWheelMove();
	float zoomStrength = 5.0f;
	if (wheel != 0)
	{
		UpdateCameraPro(&cam, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, -wheel * zoomStrength);
	}
}