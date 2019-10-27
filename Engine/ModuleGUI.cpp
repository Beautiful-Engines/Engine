#include "Application.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl3.h"
#include "ModuleGUI.h"

#include "WindowEngine.h"
#include "WindowHierarchy.h"
#include "WindowConfig.h"
#include "WindowAbout.h"
#include "WindowConsole.h"
#include "WindowPrimitives.h"
#include "WindowProperties.h"

#include "Primitive.h"

ModuleGUI::ModuleGUI(bool start_enabled) : Module(start_enabled)
{
	name = "gui";
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
	window_console = new WindowConsole();
	window_primitives = new WindowPrimitives();
	window_properties = new WindowProperties();

	// Push windows into vector
	windows_engine.push_back(window_hierarchy);
	windows_engine.push_back(window_config);
	windows_engine.push_back(window_about);
	windows_engine.push_back(window_console);
	windows_engine.push_back(window_primitives);
	windows_engine.push_back(window_properties);

	// Initialize ImGUi
	LOG("Creating ImGui context");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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

	ImGui::EndFrame();
	
	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	LOG("Cleaning GUI");

	while( windows_engine.size() > 0) 
	{ 
		windows_engine[windows_engine.size() - 1] = nullptr;
		windows_engine.pop_back();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

// Private Methods-------------------------
update_status ModuleGUI::CreateMainMenuBar()
{
	//// Docking
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", NULL, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	// Main Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		ImGuiWindowFlags aboutFlags = 0;
		aboutFlags |= ImGuiWindowFlags_NoFocusOnAppearing;

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "ALT+F4")) { return UPDATE_STOP; }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{

			if (ImGui::BeginMenu("Render Mode"))
			{
				if (ImGui::Checkbox("Depth Test", &depth_test))
					if(depth_test)
						glEnable(GL_DEPTH_TEST);
					else
						glDisable(GL_DEPTH_TEST);

				if (ImGui::Checkbox("Cull Face", &cull_face))
					if (cull_face)
						glEnable(GL_CULL_FACE);
					else
						glDisable(GL_CULL_FACE);

				if (ImGui::Checkbox("Lighting", &lighting))
					if (lighting)
						glEnable(GL_LIGHTING);
					else
						glDisable(GL_LIGHTING);

				if (ImGui::Checkbox("Color Material", &color_material))
					if (color_material)
						glEnable(GL_COLOR_MATERIAL);
					else
						glDisable(GL_COLOR_MATERIAL);

				if (ImGui::Checkbox("Texture 2D", &texture_2D))
					if (texture_2D)
						glEnable(GL_TEXTURE_2D);
					else
						glDisable(GL_TEXTURE_2D);

				if (ImGui::Checkbox("Normals", &normals))
					App->renderer3D->normals = normals;

				
				if (normals)
				{
					ImGui::Separator();
					if (ImGui::BeginMenu("Normals"))
					{
						if (ImGui::MenuItem("Vertex Normals"))
							App->renderer3D->vertex_normals = true;
						if (ImGui::MenuItem("Face Normals"))
							App->renderer3D->vertex_normals = false;

						ImGui::EndMenu();
					}
				}

				ImGui::Separator();
				if (ImGui::BeginMenu("Wireframe"))
				{
					if (ImGui::MenuItem("Fill"))
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					if (ImGui::MenuItem("Line"))
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					if (ImGui::MenuItem("Point"))
						glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

					ImGui::EndMenu();
				}
				
				ImGui::EndMenu();

			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::BeginMenu("3D Objects"))
			{
				if (ImGui::MenuItem("Sphere"))
					App->scene->AddGameObject(new Primitive(PrimitiveType::SPHERE));
				if (ImGui::MenuItem("Cube"))
					App->scene->AddGameObject(new Primitive(PrimitiveType::CUBE));
				if (ImGui::MenuItem("Torus"))
					App->scene->AddGameObject(new Primitive(PrimitiveType::TORUS));
				if (ImGui::MenuItem("Octahedron"))
					App->scene->AddGameObject(new Primitive(PrimitiveType::OCTAHEDRON));
				if (ImGui::MenuItem("Dodecahedron"))
					App->scene->AddGameObject(new Primitive(PrimitiveType::DODECAHEDRON));
				if (ImGui::MenuItem("Icosahedron"))
					App->scene->AddGameObject(new Primitive(PrimitiveType::ICOSAHEDRON));
				if (ImGui::MenuItem("Rock"))
					App->scene->AddGameObject(new Primitive(PrimitiveType::ROCK));
				if (ImGui::MenuItem("Klein Bottle"))
					App->scene->AddGameObject(new Primitive(PrimitiveType::KLEIN_BOTTLE));
					
				ImGui::EndMenu();
			}
			ImGui::Checkbox("Object Properties", &window_properties->enabled);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::Checkbox("Configuration", &window_config->enabled);
			ImGui::Checkbox("Console", &window_console->enabled);
			ImGui::Checkbox("Hierarchy", &window_hierarchy->enabled);
			ImGui::Checkbox("Demo Window", &demo);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::Checkbox("About", &window_about->enabled);
			ImGui::EndMenu();
		}

	}
	ImGui::EndMainMenuBar();

	for (uint i = 0; i < windows_engine.size(); ++i)
	{
		if (windows_engine[i]->enabled)	windows_engine[i]->Draw();
	}

	if (demo)
		ImGui::ShowDemoWindow(&demo);

	ImGui::End();

	return UPDATE_CONTINUE;
}

void ModuleGUI::LogInput(int key, const char* state, bool mouse)
{
	
	window_config->LogInput(key, state, mouse);
}

void ModuleGUI::LogDebug(const char* text)
{
	if(window_console)
		window_console->LogDebug(text);
}