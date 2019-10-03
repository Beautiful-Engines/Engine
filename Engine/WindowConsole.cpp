#include "Application.h"
#include "WindowConsole.h"

#include "ImGui\imgui_stdlib.h"

WindowConsole::WindowConsole() : WindowEngine() 
{
	enabled = true;
}

WindowConsole::~WindowConsole() {}

bool WindowConsole::Draw()
{
	ImGuiWindowFlags aboutFlags = 0;
	aboutFlags |= ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin("Console", &enabled, aboutFlags))
	{
		ImGui::TextUnformatted(buff_debug.begin());
		if (scrolling)
		{
			ImGui::SetScrollHereY(1.0f);
			scrolling = false;
		}
	}
	
	ImGui::End();

	return true;
}

void WindowConsole::LogDebug(const char* text)
{
	buff_debug.appendf(text);
	scrolling = true;
}