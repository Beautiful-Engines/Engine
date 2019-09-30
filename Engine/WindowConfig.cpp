#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"
#include "WindowConfig.h"

#include "ImGui\imgui_stdlib.h"
#include "glew\glew.h"

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
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Set Defaults"))
			{
				App->LoadDefaultConfig();
			}
			if (ImGui::MenuItem("Load"))
			{
				App->LoadConfig();
			}
			if (ImGui::MenuItem("Save"))
			{
				App->SaveConfig();
			}
			ImGui::End();
		}
		if (ImGui::CollapsingHeader("Application"))
		{
			if (ImGui::InputText("Name", app_name, 20, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
				App->SetName(app_name);

			if (ImGui::InputText("Organization", org_name, 20, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
				App->SetOrganization(org_name);
			
			if (ImGui::SliderInt("Cap FPS", &fpscap, 0, 144))
				App->SetFPSCap(fpscap);

			ImGui::Text("Framerate Cap:");
			ImGui::SameLine();
			ImGui::TextColored({ 255,255,0,255 }, "%i",App->GetFPSCap());

			
			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", App->GetFPSVector()[App->GetFPSVector().size() - 1]);
			ImGui::PlotHistogram("##framerate", &App->GetFPSVector()[0], App->GetFPSVector().size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Milliseconds %0.1f", App->GetLastFrameMSVector()[App->GetLastFrameMSVector().size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &App->GetLastFrameMSVector()[0], App->GetLastFrameMSVector().size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));

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
			ImGui::SameLine(); 
			ImGui::TextColored({ 255, 255, 0, 255 }, "%i, %i", App->input->GetMouseX(), App->input->GetMouseY());

			ImGui::Text("Mouse Motion:");
			ImGui::SameLine(); 
			ImGui::TextColored({ 255, 255, 0, 255 }, "%i, %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());

			ImGui::Text("Mouse Wheel:");
			ImGui::SameLine(); 
			ImGui::TextColored({ 255, 255, 0, 255 }, "%i", App->input->GetMouseZ());

			ImGui::Separator();
			ImGui::Text("Input LOGS:");

			ImGuiWindowFlags scrollFlags = 0;
			scrollFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

			if (ImGui::BeginChild("scroll", ImVec2(0, 0), false, scrollFlags))
			{
				ImGui::TextUnformatted(buff.begin());
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
			ImGui::TextColored({ 255, 255, 0, 255 }, "%i Gb", SDL_GetSystemRAM()/1024);

			ImGui::Text("Caps:"); 
			ImGui::SameLine();
			ImGui::TextColored({ 255, 255, 0, 255 }, "%s%s%s%s%s%s%s%s%s%s%s", (SDL_HasAVX()) ? "AVX " : "", (SDL_HasAVX2()) ? "AVX2 " : "", (SDL_HasAltiVec()) ? "AltiVec " : "",
				(SDL_Has3DNow()) ? "3DNow " : "", (SDL_HasMMX()) ? "MMX " : "", (SDL_HasRDTSC()) ? "RDTSC " : "", (SDL_HasSSE()) ? "SEE " : "",
				(SDL_HasSSE2()) ? "SSE2 " : "", (SDL_HasSSE3()) ? "SSE3 " : "", (SDL_HasSSE41()) ? "SSE41 " : "",
				(SDL_HasSSE42()) ? "SSE42 " : "");

			ImGui::Separator();

			ImGui::Text("GPU:");
			ImGui::SameLine();
			ImGui::TextColored({ 255, 255, 0, 255 }, "Vendor %s", (const char*)glGetString(GL_VENDOR));

			ImGui::Text("Brand:");
			ImGui::SameLine();
			ImGui::TextColored({ 255, 255, 0, 255 }, (const char*)glGetString(GL_RENDERER));
			
			ImGui::Text("VRAM Budget: ");
			ImGui::SameLine();
			int temp = 0;
			glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &temp);
			ImGui::TextColored({ 255, 255, 0, 255 }, "%i Mb", temp/1024);

			ImGui::Text("VRAM Usage: ");
			ImGui::SameLine();
			glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &temp);
			ImGui::TextColored({ 255, 255, 0, 255 }, "%i Mb", temp / 1024);

			ImGui::Text("VRAM Available: ");
			ImGui::SameLine();
			glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &temp);
			ImGui::TextColored({ 255, 255, 0, 255 }, "%i Mb", temp / 1024);

			ImGui::Text("VRAM Reserved: ");
			ImGui::SameLine();
			glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &temp);
			ImGui::TextColored({ 255, 255, 0, 255 }, "%i Mb", temp / 1024);


		}

		ImGui::End();

	}
	
	
	return true;
}


