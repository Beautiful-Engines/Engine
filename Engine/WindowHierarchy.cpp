#include "Application.h"
#include "ModuleScene.h"
#include"GameObject.h"
#include "ModuleGUI.h"
#include "WindowHierarchy.h"

#include "ImGui/imgui_stdlib.h"
#include "imgui/imgui_internal.h"

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

		select_iterator = 0;

		for (int i = 0; i < App->scene->GetGameObjects()[0]->GetChildren().size(); i++)
		{
			DrawNode(App->scene->GetGameObjects()[0]->GetChildren()[i]);
		}
	}
	ImGui::End();

	return true;
}

void WindowHierarchy::DrawNode(GameObject * go)
{
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	static int selection_mask = (1 << 2);
	node_clicked = -1;
	select_iterator++;

	ImGuiTreeNodeFlags node_flags = base_flags;
	//const bool is_selected = (selection_mask & (1 << select_iterator)) != 0;
	if (App->scene->GetSelected()/*is_selected*/)
	{
		if (App->scene->GetSelected() == go)
		{
			node_flags |= ImGuiTreeNodeFlags_Selected;
		}
		if (go->GetChildren().size() > 0)
		{
			if (go->IsChild(App->scene->GetSelected()))
				ImGui::SetNextTreeNodeOpen(true);
		}
	}
	if (go->GetChildren().size() > 0)
	{
		bool node_open = ImGui::TreeNodeEx(go->GetName().c_str(), node_flags);

		DragAndDrop(go);

		if (ImGui::IsItemClicked()) {
			App->scene->SetSelected(go);
			App->scene->ChangeSelected(go);
			node_flags |= ImGuiTreeNodeFlags_Selected;
			node_clicked = select_iterator;
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
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		ImGui::TreeNodeEx(go->GetName().c_str(), node_flags);

		DragAndDrop(go);

		if (ImGui::IsItemClicked()) {
			App->scene->SetSelected(go);
			App->scene->ChangeSelected(go);
			node_flags |= ImGuiTreeNodeFlags_Selected;
			SelectHierarchyItem(select_iterator, go);
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

void WindowHierarchy::DragAndDrop(GameObject* _go)
{
	// Source
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("GameObject", &_go, sizeof(GameObject));
		ImGui::EndDragDropSource();
	}

	// Target
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			GameObject* game_object = *(GameObject**)payload->Data;
			if (!game_object->IsChild(_go) && game_object != _go)
			{
				game_object->GetParent()->DeleteChild(game_object);
				game_object->SetParent(_go);
			}
		}
		ImGui::EndDragDropTarget();
	}

	// To Root
	if (ImGui::BeginDragDropTargetCustom(ImGui::GetCurrentWindow()->Rect(), (ImGuiID)"Hierarchy"))
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			GameObject* game_object = *(GameObject**)payload->Data;

			if (!game_object->IsChild(App->scene->GetGameObjects().at(0)))
			{
				game_object->GetParent()->DeleteChild(game_object);
				game_object->SetParent(App->scene->GetGameObjects().at(0));
			}
		}
		ImGui::EndDragDropTarget();
	}
}

void WindowHierarchy::SelectHierarchyItem(int i, GameObject* go)
{
	node_clicked = i;
	App->scene->SetSelected(go);
	App->scene->ChangeSelected(go);
}
