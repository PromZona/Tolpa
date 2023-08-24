#include "UI/ConsoleGui.hpp"

#include <sstream>
#include <string>

#include "imgui/rlImGui.h"
#include "Logger/Logger.hpp"
#include "Commands/CreateHumanCommand.hpp"
#include "Commands/CreateOrcCommand.hpp"
#include "game.hpp"


#define REGISTER_COMMAND(commandName, commandFunction) \
	m_commands.insert(commandName); \
	m_commandToFunction.insert({commandName, commandFunction});

void CreateHuman(const std::stringstream& commands)
{

}


ConsoleGui::ConsoleGui()
{
	REGISTER_COMMAND("CreateHuman", CreateHuman)
}

ConsoleGui::~ConsoleGui() = default;

void ConsoleGui::PushColor(LogType logType)
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

void ConsoleGui::PopColor()
{
	ImGui::PopStyleColor();
}

// True = show, False = do not show
bool ConsoleGui::FilterLogMessage(const LogMessage& message)
{
	switch (message.LogType)
	{
	case Info:
		return m_showInfoLog;
	case Warning:
		return m_showWarningLog;
	case Error:
		return m_showErrorLog;
	case Raylib:
		return true;
	}
	return false;
}

int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
{
	return 0;
}

void ConsoleGui::Draw()
{
	if (!m_showConsoleWindow)
		return;

	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Console", &m_showConsoleWindow))
	{
		ImGui::End();
		return;
	}

	ImGui::Checkbox("Auto-scroll", &m_autoScroll);
	ImGui::SameLine();

	if (ImGui::BeginPopup("Log Type"))
	{
		ImGui::Text("Filter log message types");
		ImGui::Checkbox("Info", &m_showInfoLog);
		ImGui::Checkbox("Warning", &m_showWarningLog);
		ImGui::Checkbox("Error", &m_showErrorLog);
		ImGui::EndPopup();
	}

	if (ImGui::Button("Log Type"))
		ImGui::OpenPopup("Log Type");

	ImGui::Separator();

	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
		auto& logs = Logger::GetLog();
		for (auto& log : logs)
		{
			if (!FilterLogMessage(log))
				continue;

			PushColor(log.LogType);
			ImGui::TextUnformatted(log.Message.c_str());
			PopColor();
		}
		ImGui::PopStyleVar();

		if (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		{
			ImGui::SetScrollHereY(1.0f);
		}
	}
	ImGui::EndChild();
	ImGui::Separator();

	// Command-line
	bool reclaim_focus = false;
	ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
	if (ImGui::InputText("Input", m_ConsoleInputBuf, IM_ARRAYSIZE(m_ConsoleInputBuf), input_text_flags,
		&TextEditCallbackStub, (void*)this))
	{
		SendCommand(m_ConsoleInputBuf);
		strcpy_s(m_ConsoleInputBuf, "");
		reclaim_focus = true;
	}

	// Auto-focus on window apparition
	ImGui::SetItemDefaultFocus();
	if (reclaim_focus)
		ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

	ImGui::End();
}

void ConsoleGui::SendCommand(const char* commandInput)
{
	std::stringstream commandStream(commandInput);

	std::string command;

	if (!(commandStream >> command))
	{
		Logger::Log(Info, "Failed to parse command");
		return;
	}

	if (command == "CreateHuman")
	{
		float x, y, z;
		std::unique_ptr<CreateHumanCommand> commandPtr;
		if (!(commandStream >> x >> y >> z))
			commandPtr = std::make_unique<CreateHumanCommand>();
		else
			commandPtr = std::make_unique<CreateHumanCommand>(Vector3 {x, y, z});
		Game::Instance().GetCommandManager().AddCommand(std::move(commandPtr));
	}
	else if (command == "CreateOrc")
	{
		Game::Instance().GetCommandManager().AddCommand(std::make_unique<CreateOrcCommand>());
	}
	else
	{
		Logger::Log(Info, "Unknown command met. Check syntax, or register command");
	}
}

void ConsoleGui::ShowOrHide()
{
	m_showConsoleWindow = !m_showConsoleWindow;
}
