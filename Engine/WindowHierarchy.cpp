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
	/*ImGuiWindowFlags aboutFlags = 0;
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
	
	ImGui::End();*/

	ImGui::Begin("Hierarchy");

	node_iterator = 0;

	for (int i = 0; i < App->scene->GetGameObjects()[0]->GetChildren().size(); i++)
	{
		DrawNode(App->scene->GetGameObjects()[0]->GetChildren()[i]);
	}

	ImGui::End();


	return true;
}

void WindowHierarchy::DrawNode(GameObject * go)
{
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	static int selection_mask = (1 << 2);
	static int node_clicked = -1;
	node_iterator++;

	ImGuiTreeNodeFlags node_flags = base_flags;
	const bool is_selected = (selection_mask & (1 << node_iterator)) != 0;
	if (is_selected)
		node_flags |= ImGuiTreeNodeFlags_Selected;
	if (go->GetChildren().size() > 0)
	{
		bool node_open = ImGui::TreeNodeEx(go->GetName().c_str(), node_flags);
		if (ImGui::IsItemClicked()) {
			node_clicked = node_iterator;
			App->scene->SetSelected(go);
			App->scene->ChangeSelected(go);
		}
		if (node_open) {
			for (int i = 0; i < go->GetChildren().size(); i++)
			{
				DrawNode(go->GetChildren()[i]);
			}
			ImGui::TreePop();
		}

	}
	else {
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
		ImGui::TreeNodeEx(go->GetName().c_str(), node_flags);
		if (ImGui::IsItemClicked()) {
			node_clicked = node_iterator;
			App->scene->SetSelected(go);
			App->scene->ChangeSelected(go);
		}
	}

	if (node_clicked != -1)
	{
		// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
		if (ImGui::GetIO().KeyCtrl)
			selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
			selection_mask = (1 << node_clicked);           // Click to single-select
	}
}
