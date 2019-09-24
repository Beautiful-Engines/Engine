#include "Application.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl3.h"
#include "ModuleGUI.h"

#include "EngineWindow.h"
#include "HierarchyWindow.h"

ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Init()
{
	hierarchy_window = new HierarchyWindow();
	engine_windows.push_back(hierarchy_window);

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

	for (uint i = 0; i < engine_windows.size(); ++i) { RELEASE(engine_windows[i]) };

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

// Private Methods-------------------------
int edittest = 0;
update_status ModuleGUI::CreateMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit")) { return UPDATE_STOP; }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			//TODO put strings at xml file
			if (ImGui::MenuItem("Cut", "Ctrl+X"))	{ /*TODO put cut function*/ edittest = 1; }
			if (ImGui::MenuItem("Copy", "Ctrl+C"))	{ /*TODO put copy function*/	edittest = 2; }
			if (ImGui::MenuItem("Paste", "Ctrl+V"))	{ /*TODO put paste function*/ edittest = 3; }
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
			if (ImGui::MenuItem("Hierarchy", "ALT+H")) { hierarchy_window->Show_NotShow(); }

			ImGui::Checkbox("Demo Window", &demo);

			if (ImGui::CollapsingHeader("Windows"))
			{
				ImGui::Checkbox("Window1", &window1);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (demo)
		ImGui::ShowDemoWindow(&demo);

	if (window1)
		ShowWindow1(&window1);

	switch (edittest)
	{
	case 0:
		ImGui::Text("Hello, world! \n\nTry to do something with Edit menu.");
		break;
	case 1:
		ImGui::Text("OH NO, Le has cortado los huevos al motor");
		break;
	case 2:
		ImGui::Text("No copies codigo!");
		break;
	case 3:
		ImGui::Text("El motor te responde pegandote una ostia.");
		break;
	default:
		break;
	}

	return UPDATE_CONTINUE;
}

void ModuleGUI::ShowWindow1(bool *window)
{
	static bool no_titlebar = false;
	static bool no_scrollbar = false;
	static bool no_menu = false;
	static bool no_move = false;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = false;
	static bool no_docking = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (no_docking)         window_flags |= ImGuiWindowFlags_NoDocking;
	if (no_close)           window = NULL; // Don't pass our bool* to Begin

	// Main body of the Demo window starts here.
	if (!ImGui::Begin("Window1", window, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::End();

}
