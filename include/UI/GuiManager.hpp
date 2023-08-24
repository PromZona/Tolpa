#pragma once

#include <typeindex>
#include "ECS/ECSTypes.hpp"
#include "Logger/LogTypes.hpp"
#include "ConsoleGui.hpp"

class GUIManager
{
public:
	GUIManager();
	~GUIManager();

	void Init();
	void DrawGUI();
	void Shutdown();

	void SwitchConsoleWindow();
	void SwitchHubWindow();
private:
	void DrawHub();
	void DrawECS();
	void DrawSceneSettings();
	void DrawRenderDebug();
	void HandleComponent(std::type_index type, EntityId entity);
	bool m_showHub = false;

	ConsoleGui m_ConsoleGui;
	bool ShowRenderSettings = false;
	bool ShowECSSettings = false;
};
