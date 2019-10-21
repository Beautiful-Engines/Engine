#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"
#include "WindowProperties.h"

#include "ImGui\imgui_stdlib.h"


WindowProperties::WindowProperties() : WindowEngine()
{
	enabled = true;
}


WindowProperties::~WindowProperties()
{
}

bool WindowProperties::Draw()
{
	GameObject *go = nullptr;
	ComponentTransform *trans = nullptr;
	ComponentMesh *mesh = nullptr;
	ComponentMaterial *material = nullptr;

	for (uint i = 0; i < App->scene->GetGameObjects().size(); ++i)
	{
		if (App->scene->GetGameObjects()[i]->IsFocused())
		{
			go = App->scene->GetGameObjects()[i];
		}
	}

	if (go != nullptr)
	{
		for (uint i = 0; i < go->GetComponents().size(); ++i)
		{
			if (go->GetComponents()[i]->GetType() == ComponentType::TRANSFORM)
			{
				trans = (ComponentTransform*)go->GetComponents()[i];
			}
		}
		
		for (uint i = 0; i < go->GetComponents().size(); ++i)
		{
			if (go->GetComponents()[i]->GetType() == ComponentType::MESH)
			{
				mesh = (ComponentMesh*)go->GetComponents()[i];
			}
		}

		for (uint i = 0; i < go->GetComponents().size(); ++i)
		{
			if (go->GetComponents()[i]->GetType() == ComponentType::MATERIAL)
			{
				material = (ComponentMaterial*)go->GetComponents()[i];
			}
		}
	}

	ImGuiWindowFlags aboutFlags = 0;
	aboutFlags |= ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin("Properties", &enabled, aboutFlags))
	{
		ImGui::SetWindowFontScale(1);
		if (ImGui::CollapsingHeader("Transform"))
		{
			if (trans != nullptr)
			{
				ImGui::Text("Position"); ImGui::SameLine();
				ImGui::Text("X:"); ImGui::SameLine();  ImGui::PushItemWidth(60);
				ImGui::InputFloat("", &trans->local_position.x); ImGui::SameLine();
				ImGui::Text("Y:"); ImGui::SameLine();
				ImGui::InputFloat("", &trans->local_position.y); ImGui::SameLine();
				ImGui::Text("Z:"); ImGui::SameLine();
				ImGui::InputFloat("", &trans->local_position.z);

				ImGui::Text("Rotation"); ImGui::SameLine();
				ImGui::Text("X:"); ImGui::SameLine();  ImGui::PushItemWidth(60);
				ImGui::InputFloat("", &trans->rotation.x); ImGui::SameLine();
				ImGui::Text("Y:"); ImGui::SameLine();
				ImGui::InputFloat("", &trans->rotation.y); ImGui::SameLine();
				ImGui::Text("Z:"); ImGui::SameLine();
				ImGui::InputFloat("", &trans->rotation.z);

				ImGui::Text("Size"); ImGui::SameLine();
				ImGui::Text("X:"); ImGui::SameLine();  ImGui::PushItemWidth(60);
				ImGui::InputFloat("", &trans->scale.x); ImGui::SameLine();
				ImGui::Text("Y:"); ImGui::SameLine();
				ImGui::InputFloat("", &trans->scale.y); ImGui::SameLine();
				ImGui::Text("Z:"); ImGui::SameLine();
				ImGui::InputFloat("", &trans->scale.z);
			}
			
		}
		if (ImGui::CollapsingHeader("Mesh"))
		{
			if (mesh != nullptr)
			{
				ImGui::Text("Triangle Count:");
				ImGui::SameLine();
				ImGui::TextColored({ 255, 255, 0, 255 }, "%i", mesh->n_indexes / 3);
			}
		}
		if (ImGui::CollapsingHeader("Texture"))
		{
			if (material != nullptr)
			{
				ImGui::Text("Texture Size:");
				ImGui::SameLine();
				ImGui::TextColored({ 255, 255, 0, 255 }, "%i * %i", material->width, material->height);
			}
			
		}
	}

	ImGui::End();

	return true;
}
