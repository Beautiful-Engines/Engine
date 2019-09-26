#include "Application.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl3.h"
#include "ModuleGUI.h"

#include "WindowEngine.h"
#include "WindowHierarchy.h"
#include "WindowConfig.h"
#include "WindowAbout.h"

ModuleGUI::ModuleGUI(bool start_enabled) : Module(start_enabled)
{
}


ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Init()
{
	// Creating windows
	window_hierarchy = new WindowHierarchy();
	window_config = new WindowConfig();
	window_about = new WindowAbout();

	// Push windows into vector
	windows_engine.push_back(window_hierarchy);
	windows_engine.push_back(window_config);
	windows_engine.push_back(window_about);

	// Initialize ImGUi
	LOG("Creating ImGui context");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	return true;
}

update_status ModuleGUI::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	// Create main menu bar
	ret = CreateMainMenuBar();

	return ret;
}

update_status ModuleGUI::PostUpdate(float dt)
{
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	LOG("Cleaning GUI");

	for (uint i = 0; i < windows_engine.size(); ++i) { RELEASE(windows_engine[i]) };

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

// Private Methods-------------------------
update_status ModuleGUI::CreateMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "ALT+F4")) { return UPDATE_STOP; }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			//TODO put strings at xml file
			if (ImGui::MenuItem("Cut", "Ctrl+X"))	{ /*TODO put cut function*/ }
			if (ImGui::MenuItem("Copy", "Ctrl+C"))	{ /*TODO put copy function*/}
			if (ImGui::MenuItem("Paste", "Ctrl+V"))	{ /*TODO put paste function*/}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Assets"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Component"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Hierarchy", "ALT+H")) 
			{

				window_hierarchy->Show_NotShow(); 
			}

			ImGui::Checkbox("Demo Window", &demo);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Configuration", "ALT+C")) { window_config->Show_NotShow(); }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About", "ALT+A")) { window_about->Show_NotShow(); }
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	for (uint i = 0; i < windows_engine.size(); ++i)
	{
		if (windows_engine[i]->IsEnabled())	windows_engine[i]->Draw();
	}

	if (demo)
		ImGui::ShowDemoWindow(&demo);

	return UPDATE_CONTINUE;
}

