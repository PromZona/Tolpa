#pragma once

#include <typeindex>
#include "ECS/ECSTypes.hpp"
#include "Logger/LogTypes.hpp"

class GUIManager
{
public:
	GUIManager();
	~GUIManager();

	void Init();
	void DrawGUI();
	void Shutdown();
private:
	void DrawECS();
	void HandleComponent(std::type_index type, EntityId entity);
	void DrawConsole();

	void PushColor(LogType logType);
	void PopColor();

	bool m_showEcsWindow = true;
	bool m_showConsoleWindow = true;

	char m_ConsoleInputBuf[256];
};
