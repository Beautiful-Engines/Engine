#include "Application.h"
#include "ModuleScene.h"
#include"GameObject.h"
#include "ModuleGUI.h"
#include "WindowHierarchy.h"

#include "ImGui\imgui_stdlib.h"

WindowHierarchy::WindowHierarchy() : WindowEngine() 
{
	enabled = true;
}

WindowHierarchy::~WindowHierarchy() {}

bool WindowHierarchy::Draw()
{
	ImGuiWindowFlags aboutFlags = 0;
	aboutFlags |= ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin("Hierarchy", &enabled, aboutFlags))
	{
		static int selected = 0;
		
		for (int i = 0; i < App->scene->GetGameObjects().size(); ++i)
		{
			if (App->scene->GetGameObjects()[i]->GetName() != "root")
			{
				ImGui::Selectable(App->scene->GetGameObjects()[i]->GetName().c_str());
				if (ImGui::IsItemClicked(0))
				{
					App->scene->GetGameObjects()[i]->SetFocus(true);
					for (int j = 0; j < App->scene->GetGameObjects().size(); ++j)
					{
						if(j != i)App->scene->GetGameObjects()[j]->SetFocus(false);
					}
				}
			}
		}
	}
	
	ImGui::End();
	return true;
}
