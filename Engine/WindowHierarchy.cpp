#include "Application.h"
#include "ModuleScene.h"
#include"GameObject.h"
#include "ModuleGUI.h"
#include "WindowHierarchy.h"

#include "ImGui\imgui_stdlib.h"

WindowHierarchy::WindowHierarchy() : WindowEngine() {}

WindowHierarchy::~WindowHierarchy() {}

bool WindowHierarchy::Draw()
{

	if (ImGui::Begin("Hierarchy"))
	{
		static int selected = 0;
		
		for (int i = 0; i < App->scene->GetGameObjects().size(); ++i)
		{
			if (App->scene->GetGameObjects()[i]->GetName() != "root")
			{
				ImGui::Selectable(App->scene->GetGameObjects()[i]->GetName().c_str());
			}
		}

		ImGui::End();
	}
	return true;
}
