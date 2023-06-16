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

	DrawHub();
	rlImGuiEnd();
}

void GUIManager::Init()
{
	rlImGuiSetup(true);
}

void GUIManager::DrawHub()
{
	ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("GUI Manager", &m_showEcsWindow))
	{
		ImGui::End();
		return;
	}

	if (ImGui::TreeNode("Render Settings"))
	{
		DrawRenderDebug();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("ECS Entities"))
	{
		DrawECS();
		ImGui::TreePop();
	}

	ImGui::End();
}

void GUIManager::DrawRenderDebug()
{
	float spacing = ImGui::GetStyle().ItemSpacing.x; 

	auto& gameInstance = Game::Instance();
	auto& testPoint = gameInstance.GetNavGrid().GetTestPoint();
	auto& rendererGlobalFlags = gameInstance.GetRendererScene().GetFlags();
	auto& rendererGlobalVariables = gameInstance.GetRendererScene().GetDebugVariables();
	
	ImGui::SeparatorText("Scene Flags");
	ImGui::Checkbox("NavMesh Wireframe", &rendererGlobalFlags.drawDebugNavMeshWireframe);
	ImGui::Checkbox("NavMesh Midpoints", &rendererGlobalFlags.drawDebugNavMeshMiddlePoints);
	ImGui::Checkbox("NavMesh Graph", &rendererGlobalFlags.drawDebugNavMeshGraph);
	ImGui::Checkbox("NavMesh KD-Tree", &rendererGlobalFlags.drawDebugNavMeshKDTree);
	
	if (rendererGlobalFlags.drawDebugNavMeshKDTree)
	{
		ImGui::Separator();

		if(ImGui::Checkbox("Steps", &rendererGlobalFlags.drawDebugNavMeshKDTreeInverted))
		{
			rendererGlobalVariables.KDTreeDepthDrawingDepth = 0;
			rendererGlobalFlags.drawDebugNavMeshKDTreeElevated = false;
		}
		ImGui::SameLine(0, spacing);
		
		if(ImGui::Checkbox("Elevated", &rendererGlobalFlags.drawDebugNavMeshKDTreeElevated))
			rendererGlobalFlags.drawDebugNavMeshKDTreeInverted = false;

		if (ImGui::ArrowButton("##DecreaseDepth", ImGuiDir_Left) && 
		rendererGlobalVariables.KDTreeDepthDrawingDepth > 0)
			rendererGlobalVariables.KDTreeDepthDrawingDepth--;
		ImGui::SameLine(0, spacing);

		if (ImGui::ArrowButton("##IncreaseDepth", ImGuiDir_Right) && 
		rendererGlobalVariables.KDTreeDepthDrawingDepth <
		gameInstance.GetNavGrid().GetNavKDTree().GetMaxDepth() - 1)
			rendererGlobalVariables.KDTreeDepthDrawingDepth++;
		ImGui::SameLine(0, spacing);

		ImGui::Text("Depth: %d", rendererGlobalVariables.KDTreeDepthDrawingDepth);
		
		ImGui::Separator();
	}

	ImGui::Checkbox("NavMesh KD-Tree Nearest Points", &rendererGlobalFlags.drawDebugNavMeshNearestPoint);

	if (rendererGlobalFlags.drawDebugNavMeshNearestPoint)
	{
		ImGui::Separator();
		static float controlStrength = 5.0f;
		ImGui::SliderFloat("Strength", &controlStrength, 1.0f, 10.0f);

		if (ImGui::Button(" +X "))
			testPoint.x += controlStrength;
		ImGui::SameLine(0, spacing);
		if (ImGui::Button(" -X "))
			testPoint.x -= controlStrength;

		if (ImGui::Button(" +Y "))
			testPoint.y += controlStrength;
		ImGui::SameLine(0, spacing);
		if (ImGui::Button(" -Y "))
			testPoint.y -= controlStrength;

		if (ImGui::Button(" +Z "))
			testPoint.z += controlStrength;
		ImGui::SameLine(0, spacing);
		if (ImGui::Button(" -Z "))
			testPoint.z -= controlStrength;

		ImGui::Separator();
	}

	ImGui::Checkbox("Terrain Wireframe", &rendererGlobalFlags.drawDebugTerrainWireframe);

	ImGui::SeparatorText("Unit Flags");
	ImGui::Checkbox("Unit Path", &gameInstance.GetRendererUnits().GetFlags().drawDebugPath);
	ImGui::Checkbox("Unit ForwardVector (TODO)", &gameInstance.GetRendererUnits().GetFlags().drawDebugForwardVector);
}

void GUIManager::DrawECS()
{
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
				ImGui::InputFloat3("Position", &comp->Position.x);
			}
		}

		if (type == typeid(GoalComponent))
		{
			auto* comp = Game::Instance().GetECS().GetComponent<GoalComponent>(entity);
			if (comp != nullptr)
			{
				ImGui::InputFloat3("Goal Position", &comp->GoalPosition.x);
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
