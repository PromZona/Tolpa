#include "UI/GuiManager.hpp"

#include <algorithm>
#include <string>

#include "imgui/rlImGui.h"
#include "game.hpp"

#include "Components/MovementComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/GoalComponent.hpp"
#include "Components/RenderComponent.hpp"

GUIManager::GUIManager() : m_ConsoleGui() {}
GUIManager::~GUIManager() = default;

void GUIManager::DrawGUI()
{
	rlImGuiBegin();

	DrawHub();
	m_ConsoleGui.Draw();
	rlImGuiEnd();
}

void GUIManager::Init()
{
	rlImGuiSetup(true);
}

void GUIManager::DrawHub()
{
	if (!m_showHub)
		return;

	ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("GUI Manager", &m_showHub))
	{
		ImGui::End();
		return;
	}

	DrawSceneSettings();
	ImGui::Separator();

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

void GUIManager::DrawSceneSettings()
{
	float spacing = ImGui::GetStyle().ItemSpacing.x; 

	auto& sceneManager = Game::Instance().GetSceneManager();
	auto& sceneFlags = sceneManager.GetSceneFlags();
	auto& sceneVariables = sceneManager.GetSceneVariables();

	ImGui::Text("Current scene: %d", sceneVariables.SceneNumber + 1);
	ImGui::SameLine(0, spacing);
	ImGui::Text("Total scenes: %d", sceneVariables.TotalScenes);

	if (ImGui::Button("Previous Scene") && sceneVariables.SceneNumber > 0)
	{
		sceneVariables.SceneNumber--;
		sceneManager.InitializeScene();
	}
	ImGui::SameLine(0, spacing);

	if (ImGui::Button("Next Scene") && sceneVariables.SceneNumber < sceneVariables.TotalScenes)
	{
		sceneVariables.SceneNumber++;
		sceneManager.InitializeScene();
	}
	ImGui::SameLine(0, spacing);

	if (ImGui::Checkbox("NoModels Mode", &sceneFlags.NoModelsMode))
		sceneManager.InitializeScene();
}

void GUIManager::DrawRenderDebug()
{
	float spacing = ImGui::GetStyle().ItemSpacing.x; 

	auto& gameInstance = Game::Instance();
	auto& testPoint = gameInstance.GetNavGrid().GetTestPoint();
	auto& rendererGlobalFlags = gameInstance.GetRendererScene().GetFlags();
	auto& rendererGlobalVariables = gameInstance.GetRendererScene().GetDebugVariables();
	auto& sceneLight = gameInstance.GetRendererScene().GetLight();
	
	ImGui::SeparatorText("Scene Flags");
	ImGui::Checkbox("Draw Terrain", &rendererGlobalFlags.DrawDebugModels);
	if (ImGui::Checkbox("Rotate Ligh Source", &rendererGlobalFlags.RotateLight))
	{
		if (sceneLight.position.x == 0.0f)
			sceneLight.position.x = 400.0f;
		else
		{
			sceneLight.position.x = 0.0f;
			sceneLight.position.z = 0.0f;
			sceneLight.color = WHITE;
		}
	}
	
	if (rendererGlobalFlags.RotateLight)
	{
		ImGui::SliderFloat("Speed", &rendererGlobalVariables.LightRotationSpeed, 0.0f, 1.0f);

		static ImVec4 colorPicked;
		if (ImGui::ColorEdit4("Color", (float*)&colorPicked))
		{
			sceneLight.color.r = static_cast<unsigned char>(colorPicked.x * 255);
			sceneLight.color.g = static_cast<unsigned char>(colorPicked.y * 255);
			sceneLight.color.b = static_cast<unsigned char>(colorPicked.z * 255);
			sceneLight.color.a = static_cast<unsigned char>(colorPicked.w * 255);
		}
	}

	ImGui::Checkbox("NavMesh Wireframe", &rendererGlobalFlags.DrawDebugNavMeshWireframe);
	ImGui::Checkbox("NavMesh Midpoints", &rendererGlobalFlags.DrawDebugNavMeshMiddlePoints);
	ImGui::Checkbox("NavMesh Graph", &rendererGlobalFlags.DrawDebugNavMeshGraph);
	ImGui::Checkbox("NavMesh KD-Tree", &rendererGlobalFlags.DrawDebugNavMeshKDTree);
	
	if (rendererGlobalFlags.DrawDebugNavMeshKDTree)
	{
		ImGui::Separator();

		if(ImGui::Checkbox("Steps", &rendererGlobalFlags.DrawDebugNavMeshKDTreeInverted))
		{
			rendererGlobalVariables.KDTreeDepthDrawingDepth = 0;
			rendererGlobalFlags.DrawDebugNavMeshKDTreeElevated = false;
		}
		ImGui::SameLine(0, spacing);
		
		if(ImGui::Checkbox("Elevated", &rendererGlobalFlags.DrawDebugNavMeshKDTreeElevated))
			rendererGlobalFlags.DrawDebugNavMeshKDTreeInverted = false;

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

	ImGui::Checkbox("NavMesh KD-Tree Nearest Points", &rendererGlobalFlags.DrawDebugNavMeshNearestPoint);

	if (rendererGlobalFlags.DrawDebugNavMeshNearestPoint)
	{
		ImGui::Separator();
		static float controlStrength = 10.0f;
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

	ImGui::Checkbox("Terrain Wireframe", &rendererGlobalFlags.DrawDebugTerrainWireframe);

	ImGui::SeparatorText("Unit Flags");
	ImGui::Checkbox("Unit Path", &gameInstance.GetRendererUnits().GetFlags().DrawDebugPath);
	ImGui::Checkbox("Unit ForwardVector (TODO)", &gameInstance.GetRendererUnits().GetFlags().DrawDebugForwardVector);
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

void GUIManager::SwitchConsoleWindow()
{
	m_ConsoleGui.ShowOrHide();
}

void GUIManager::SwitchHubWindow()
{
	m_showHub = !m_showHub;
}

