#include "GuiManager.hpp"

#include <algorithm>
#include <string>

#include "imgui/rlImGui.h"
#include "game.hpp"

#include "Components/MovementComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/GoalComponent.hpp"
#include "Components/RenderComponent.hpp"

GUIManager::GUIManager() = default;
GUIManager::~GUIManager() = default;

void GUIManager::DrawGUI()
{
	rlImGuiBegin();

	DrawECS();
	DrawRenderDebug();
	// ImGui::ShowDemoWindow();
	rlImGuiEnd();
}

void GUIManager::Init()
{
	rlImGuiSetup(true);
}

void GUIManager::DrawRenderDebug()
{
	ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Render Flags", &m_showEcsWindow))
	{
		ImGui::End();
		return;
	}

	auto& gameInstance = Game::Instance();

	ImGui::SeparatorText("Scene Flags");
	ImGui::Checkbox("NavMesh Wireframe", &gameInstance.GetRendererScene().GetFlags().drawDebugNavMeshWireframe);
	ImGui::Checkbox("NavMesh Midpoints", &gameInstance.GetRendererScene().GetFlags().drawDebugNavMeshMiddlePoints);
	ImGui::Checkbox("NavMesh Graph", &gameInstance.GetRendererScene().GetFlags().drawDebugNavMeshGraph);
	ImGui::Checkbox("Terrain Wireframe", &gameInstance.GetRendererScene().GetFlags().drawDebugTerrainWireframe);

	ImGui::SeparatorText("Unit Flags");
	ImGui::Checkbox("Unit Path", &gameInstance.GetRendererUnits().GetFlags().drawDebugPath);
	ImGui::Checkbox("Unit ForwardVector (TODO)", &gameInstance.GetRendererUnits().GetFlags().drawDebugForwardVector);

	ImGui::End();
}

void GUIManager::DrawECS()
{
	ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Entity Inspector", &m_showEcsWindow))
	{
		ImGui::End();
		return;
	}
	auto entityMap = Game::Instance().GetECS().GetEntityToArchetype();
	for (auto& pair : entityMap)
	{
		auto compMap = pair.second->GetRawComponents();

		if (ImGui::TreeNode(std::to_string(pair.first).c_str()))
		{
			for (auto& compPair : compMap)
			{
				HandleComponent(compPair.first, pair.first);
			}
			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void GUIManager::HandleComponent(std::type_index type, EntityId entity)
{
	if (ImGui::TreeNode(type.name()))
	{
		if (type == typeid(MovementComponent))
		{
			auto* comp = Game::Instance().GetECS().GetComponent<MovementComponent>(entity);
			if (comp != nullptr)
			{
				ImGui::SliderFloat2("Direction", &comp->Direction.x, -1.0, 1.0);
				ImGui::SliderFloat("Speed", &comp->Speed,0.0f, 200.0f);
			}
		}

		if (type == typeid(TransformComponent))
		{
			auto* comp = Game::Instance().GetECS().GetComponent<TransformComponent>(entity);
			if (comp != nullptr)
			{
				ImGui::InputFloat2("Position", &comp->Position.x);
			}
		}

		if (type == typeid(GoalComponent))
		{
			auto* comp = Game::Instance().GetECS().GetComponent<GoalComponent>(entity);
			if (comp != nullptr)
			{
				ImGui::InputFloat2("Goal Position", &comp->GoalPosition.x);
				ImGui::Checkbox("Is Active", &comp->IsActive);
			}
		}

		if (type == typeid(RenderComponent))
		{
			auto* comp = Game::Instance().GetECS().GetComponent<RenderComponent>(entity);
			if (comp != nullptr)
			{
				ImGui::SliderFloat("Radius", &comp->Radius, 0.0f, 50.0f);
			}
		}

		ImGui::TreePop();
	}
}
void GUIManager::Shutdown()
{
	rlImGuiShutdown();
}
