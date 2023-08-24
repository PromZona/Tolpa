#include "InputManager.hpp"

#include <raylib.h>
#include "EntityFactory.hpp"
#include "game.hpp"
#include "NavigationMesh.hpp"

#include "Components/TransformComponent.hpp"
#include "Commands/CreateCityCommand.hpp"
#include "Commands/CreateHumanCommand.hpp"
#include "Commands/CreateOrcsTribeCommand.hpp"
#include "Commands/CreateOrcCommand.hpp"

#include <random>

bool Vector3Comparator(const Vector3& v1, const Vector3& v2)
{
    return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
}

// RANDOM LOCATION ON NAV MESH GRID
// THIS IS TEMPORARY UNTIL MOUSE POSITION BECOMES AVAILABLE
// (and nav mesh + pathfinding actually works as intended)
Vector3 GetRandomLocation()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	
    auto& navGrid = Game::Instance().GetNavGrid();

	int min = 0;
	int max = navGrid.GetGraphNodes().size();

    std::uniform_int_distribution<int> dis(min, max);

	Vector3 selectedCell;
	selectedCell = navGrid.GetGraphNodes()[dis(gen)];

	return selectedCell;
}

InputManager::InputManager(){}
InputManager::~InputManager() = default;

void InputManager::Update()
{
	auto& sceneRenderer = Game::Instance().GetRendererScene();
	auto& cam = sceneRenderer.GetCamera();
	auto& sceneGameplayVariables = sceneRenderer.GetGameplayVariables();

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
			sceneGameplayVariables.ShowPlacingPreview = true;
			sceneGameplayVariables.previewModelType = ModelType::HUMAN;
			continue;
		}
    
		if (keyPressed == KEY_C)
		{
			sceneGameplayVariables.ShowPlacingPreview = true;
			sceneGameplayVariables.previewModelType = ModelType::CITY;
			continue;
		}

		if (keyPressed == KEY_T)
		{
			sceneGameplayVariables.ShowPlacingPreview = true;
			sceneGameplayVariables.previewModelType = ModelType::TRIBE;
			continue;
		}

		if (keyPressed == KEY_Z)
		{
			Game::Instance().GetECS().PrintInfo();
		}

		if (keyPressed == KEY_GRAVE)
		{
			Game::Instance().GetGUI().SwitchConsoleWindow();
		}

		if (keyPressed == KEY_ONE)
		{
			Game::Instance().GetGUI().SwitchHubWindow();
		}
	}

	if (sceneGameplayVariables.ShowPlacingPreview)
	{
		auto& terrainPicker = sceneRenderer.GetMeshPicker();
		if (terrainPicker.IsTerrainHit() && IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
		{
			switch (sceneGameplayVariables.previewModelType)
			{
				case ModelType::HUMAN:
				{
					auto command = std::make_unique<CreateHumanCommand>(terrainPicker.GetCollisionPoint().point);
					Game::Instance().GetCommandManager().AddCommand(std::move(command));
					break;
				}
				case ModelType::CITY:
				{
					auto command = std::make_unique<CreateCityCommand>(terrainPicker.GetCollisionPoint().point);
					Game::Instance().GetCommandManager().AddCommand(std::move(command));
					break;
				}
				case ModelType::TRIBE:
				{
					auto command = std::make_unique<CreateOrcsTribeCommand>(terrainPicker.GetCollisionPoint().point);
					Game::Instance().GetCommandManager().AddCommand(std::move(command));
					break;
				}
				default:
					TraceLog(LOG_ERROR, "Placing an entity failed");
					break;
			}

			sceneGameplayVariables.ShowPlacingPreview = false;
		}

		if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_RIGHT))
			sceneGameplayVariables.ShowPlacingPreview = false;
	}

	Vector2 prevMousePosition = {0.0f, 0.0f};
	Vector2 currPosition = {0.0f, 0.0f};
	Vector2 mouseDelta = {0.0f, 0.0f};

	Vector3 camPosition;
	float cameraSpeed = 0.01f;

	if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_RIGHT))
		prevMousePosition = GetMousePosition();

	if (IsMouseButtonDown(MouseButton::MOUSE_BUTTON_RIGHT) && !sceneGameplayVariables.ShowPlacingPreview)
	{
		currPosition = {GetMousePosition().x - Game::Instance().ScreenWidth / 2,
						GetMousePosition().y - Game::Instance().ScreenHeight / 2};

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
		UpdateCameraPro(&cam, Vector3Zero(), Vector3Zero(), -wheel * zoomStrength);
	}
}