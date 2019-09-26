#include "Application.h"
#include "ModuleWindow.h"
#include "WindowConfig.h"

#include <string> 

WindowConfig::WindowConfig() : WindowEngine() 
{
}

WindowConfig::~WindowConfig() {}

bool WindowConfig::Draw()
{
	ImGuiWindowFlags aboutFlags = 0;
	aboutFlags |= ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin("Configuration", &enabled, aboutFlags))
	{
		ImGui::Text("Options");
		if (ImGui::CollapsingHeader("Application"))
		{
		}
		if (ImGui::CollapsingHeader("Window"))
		{
			if (ImGui::SliderInt("Width", &width, 0, 1920))	App->window->SetWindowWidth(width);
			if (ImGui::SliderInt("Height", &height, 0, 1080)) App->window->SetWindowHeight(height);
			if (ImGui::SliderFloat("Brightness", &brightness, 0, 1)) App->window->SetBrightness(brightness);

			ImGui::Text("Refresh rate");
			ImGui::SameLine();
			std::string conv = std::to_string(App->window->GetRefreshRate());
			ImGui::TextColored({ 255, 255, 0, 255 }, conv.c_str());

			if (ImGui::Checkbox("Fullscreen", &fullscreen)) {
				App->window->SetScreenMode(FULLSCREEN, fullscreen);
			}

			ImGui::SameLine();

			if (ImGui::Checkbox("Resizable", &resizable)) {
				App->window->SetScreenMode(RESIZABLE, resizable);
			}
			if (ImGui::Checkbox("Borderless", &borderless)) {
				App->window->SetScreenMode(BORDERLESS, borderless);
			}

			ImGui::SameLine();

			if (ImGui::Checkbox("Full Desktop", &fulldesktop)) {
				App->window->SetScreenMode(FULLDESKTOP, fulldesktop);
			}
		}

		ImGui::End();

	}
	
	
	return true;
}