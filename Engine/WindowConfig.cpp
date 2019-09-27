#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
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
			ImGui::TextColored({ 255,255,0,255 }, "%i",App->GetFPSCap());

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
		if (ImGui::CollapsingHeader("File System"))
		{
		}
		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Text("Mouse Position:");
			ImGui::SameLine(); ImGui::TextColored({ 255, 255, 0, 255 }, "%i, %i", App->input->GetMouseX(), App->input->GetMouseY());

			ImGui::Text("Mouse Motion:");
			ImGui::SameLine(); ImGui::TextColored({ 255, 255, 0, 255 }, "%i, %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());

			ImGui::Text("Mouse Wheel:");
			ImGui::SameLine(); ImGui::TextColored({ 255, 255, 0, 255 }, "%i", App->input->GetMouseZ());

			ImGui::Separator();
			ImGui::Text("Input LOGS:");

			ImGuiWindowFlags scrollFlags = 0;
			scrollFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

			if (ImGui::BeginChild("scroll", ImVec2(0, 0), false, scrollFlags))
			{
				//ImGui::TextUnformatted(buff.begin());
				ImGui::SetScrollHere(1.0f);
			}
			ImGui::EndChild();
		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
			SDL_VERSION(&compiled_version);
			ImGui::Text("SDL Version:");
			ImGui::SameLine();
			ImGui::TextColored({ 255, 255, 0, 255 }, "%i.%i.%i", compiled_version.major, compiled_version.minor, compiled_version.patch);

			ImGui::Separator();

			ImGui::Text("CPU cores:");
			ImGui::SameLine();
			ImGui::TextColored({ 255, 255, 0, 255 }, "%i Cache: %i Kb", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

			ImGui::Text("System RAM:");
			ImGui::SameLine();
			ImGui::TextColored({ 255, 255, 0, 255 }, "%i Mb", SDL_GetSystemRAM());

			ImGui::Text("Caps:");
			ImGui::SameLine();
			ImGui::Text("Caps:"); ImGui::SameLine();
			ImGui::TextColored({ 255, 255, 0, 255 }, "%s%s%s%s%s%s%s%s%s%s%s", (SDL_HasAVX()) ? "AVX " : "", (SDL_HasAVX2()) ? "AVX2 " : "", (SDL_HasAltiVec()) ? "AltiVec " : "",
				(SDL_Has3DNow()) ? "3DNow " : "", (SDL_HasMMX()) ? "MMX " : "", (SDL_HasRDTSC()) ? "RDTSC " : "", (SDL_HasSSE()) ? "SEE " : "",
				(SDL_HasSSE2()) ? "SSE2 " : "", (SDL_HasSSE3()) ? "SSE3 " : "", (SDL_HasSSE41()) ? "SSE41 " : "",
				(SDL_HasSSE42()) ? "SSE42 " : "");

			ImGui::Separator();

		}

		ImGui::End();

	}
	
	
	return true;
}