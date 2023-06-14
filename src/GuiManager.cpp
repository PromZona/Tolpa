#include "GuiManager.hpp"

#include <algorithm>

#include "imgui/rlImGui.h"
#include "game.hpp"
#include "Logger/Logger.hpp"

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
	DrawConsole();
	ImGui::ShowDemoWindow();
	rlImGuiEnd();
}

void GUIManager::Init()
{
	rlImGuiSetup(true);
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

int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
{
	return 0;
}

void GUIManager::DrawConsole()
{
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Console", &m_showConsoleWindow))
	{
		ImGui::End();
		return;
	}

	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
		auto& logs = Logger::GetLog();
		for (auto& log : logs)
		{
			PushColor(log.LogType);
			ImGui::TextUnformatted(log.Message.c_str());
			PopColor();
		}
		ImGui::PopStyleVar();
	}
	ImGui::EndChild();
	ImGui::Separator();

	// Command-line
	bool reclaim_focus = false;
	ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
	if (ImGui::InputText("Input", m_ConsoleInputBuf, IM_ARRAYSIZE(m_ConsoleInputBuf), input_text_flags,
		&TextEditCallbackStub, (void*)this))
	{
		strcpy_s(m_ConsoleInputBuf, "");
	}

	// Auto-focus on window apparition
	ImGui::SetItemDefaultFocus();
	if (reclaim_focus)
		ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

	ImGui::End();
}

void GUIManager::PushColor(LogType logType)
{
	ImVec4 color;
	switch (logType)
	{
	case Info:
		color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case Warning:
		color = ImVec4(1.0f, 0.9f, 0.0f, 1.0f);
		break;
	case Error:
		color = ImVec4(0.8f, 0.1f, 0.2f, 1.0f);
		break;
	case LogType::Raylib:
		color = ImVec4(0.6f, 0.4f, 0.6f, 1.0f);
		break;
	default:
		color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	}
	ImGui::PushStyleColor(ImGuiCol_Text, color);
}

void GUIManager::PopColor()
{
	ImGui::PopStyleColor();

}
