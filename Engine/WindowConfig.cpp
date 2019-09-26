#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleGUI.h"
#include "WindowConfig.h"

#include "ImGui/imgui_stdlib.h"

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
			/*if (ImGui::InputText("Name", &app_name, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
				App->SetName(app_name);
			if (ImGui::InputText("Organization", &org_name, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
				App->SetOrganization(org_name);*/
			if (ImGui::SliderInt("Cap FPS", &fpscap, 0, 144))
				App->SetFPSCap(fpscap);

			ImGui::Text("Framerate Cap:");
			ImGui::SameLine();
			ImGui::TextColored({ 255,255,0,255 }, std::to_string(App->GetFPSCap()).c_str());

			char title[25];
			/*sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));*/

			bool vsync = App->renderer3D->GetVSync();
			if (ImGui::Checkbox("Use VSync", &vsync))
			{
				App->renderer3D->SetVSync(vsync);
				if (vsync && App->GetFPSCap()>0)
					App->SetFPSCap(fpscap);
			}
		}
		if (ImGui::CollapsingHeader("Window"))
		{
			if (ImGui::SliderInt("Width", &width, 0, 1920))	App->window->SetWindowWidth(width);
			if (ImGui::SliderInt("Height", &height, 0, 1080)) App->window->SetWindowHeight(height);
			if (ImGui::SliderFloat("Brightness", &brightness, 0, 1)) App->window->SetBrightness(brightness);

			ImGui::Text("Refresh rate");
			ImGui::SameLine();
			ImGui::TextColored({ 255, 255, 0, 255 },"%i", App->window->GetRefreshRate());

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