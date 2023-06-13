#include "InputManager.hpp"

#include <raylib.h>
#include "EntityFactory.hpp"
#include "game.hpp"

#include "Components/TransformComponent.hpp"
#include "Commands/CreateCityCommand.hpp"
#include "Commands/CreateHumanCommand.hpp"
#include "Commands/CreateOrcsTribeCommand.hpp"
#include "Commands/CreateOrcCommand.hpp"

#include <random>

// RANDOM LOCATION ON NAV MESH GRID
// THIS IS TEMPORARY UNTIL MOUSE POSITION BECOMES AVAILABLE
// (and nav mesh + pathfinding actually works as intended)
Vector3 GetRandomLocation()
{
    auto& navGrid = Game::Instance().GetNavGrid();

	std::random_device rd;
	std::mt19937 gen(rd());

	int min = 0;
	int max = navGrid.GetTriangles().size();
    std::uniform_int_distribution<int> dis(min, max);

	return navGrid.GetTriangles()[dis(gen)].middlePoint;
}

InputManager::InputManager(){}
InputManager::~InputManager() = default;

void InputManager::Update()
{
	auto& cam = Game::Instance().GetRendererScene().GetCamera();
	int keyPressed = 0;

	while ((keyPressed = GetKeyPressed()) != 0)
	{
		if (keyPressed == KEY_O)
		{
			auto command = std::make_unique<CreateOrcCommand>();
			Game::Instance().GetCommandManager().AddCommand(std::move(command));
			continue;
		}

		if (keyPressed == KEY_H)
		{
			//auto mousePos = GetMousePosition();
			auto command = std::make_unique<CreateHumanCommand>(GetRandomLocation());
			Game::Instance().GetCommandManager().AddCommand(std::move(command));
			continue;
		}
    
		if (keyPressed == KEY_C)
		{
			//auto mousePos = GetMousePosition();
			auto command = std::make_unique<CreateCityCommand>(GetRandomLocation());
			Game::Instance().GetCommandManager().AddCommand(std::move(command));
			continue;
		}

		if (keyPressed == KEY_T)
		{
			//auto mousePos = GetMousePosition();
			auto command = std::make_unique<CreateOrcsTribeCommand>(GetRandomLocation());
			Game::Instance().GetCommandManager().AddCommand(std::move(command));
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
		prevMousePosition = GetMousePosition();

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
	float zoomStrength = 7.0f;
	if (wheel != 0)
	{
		UpdateCameraPro(&cam, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, -wheel * zoomStrength);
	}
}