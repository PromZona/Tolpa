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
	void SwitchECSWindow();
private:
	void DrawECS();
	void HandleComponent(std::type_index type, EntityId entity);
	bool m_showEcsWindow = false;

	ConsoleGui m_ConsoleGui;
};
