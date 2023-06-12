#pragma once

#include <typeindex>
#include "ECS/ECSTypes.hpp"

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
	void DrawRenderDebug();
	void HandleComponent(std::type_index type, EntityId entity);

	bool m_showEcsWindow = true;
};
