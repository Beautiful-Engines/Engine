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


	go = App->scene->GetSelected();

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
				if (!mesh->checkered && material->id_texture == mesh->id_texture)
					break;
				else if (mesh->checkered && material->id_texture == mesh->id_default_texture)
					break;
			}
		}
	}

	ImGuiWindowFlags aboutFlags = 0;
	aboutFlags |= ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin("Properties", &enabled, aboutFlags))
	{
		ImGui::SetWindowFontScale(1);
		
		if (trans != nullptr)
		{
			ImGui::Checkbox("Hide", &go->hide);
			if (go->hide)
				go->Disable();
			else
				go->Enable();

			if (ImGui::CollapsingHeader("Transform"))
			{
			
					ImGui::Text("Position"); ImGui::SameLine();
					ImGui::Text("X:"); ImGui::SameLine();  ImGui::PushItemWidth(60);
					ImGui::TextColored({ 255, 255, 0, 255 }, "%f", trans->local_position.x); ImGui::SameLine();
					ImGui::Text("Y:"); ImGui::SameLine();
					ImGui::TextColored({ 255, 255, 0, 255 }, "%f", trans->local_position.y); ImGui::SameLine();
					ImGui::Text("Z:"); ImGui::SameLine();
					ImGui::TextColored({ 255, 255, 0, 255 }, "%f", trans->local_position.z);

					ImGui::Text("Rotation"); ImGui::SameLine();
					ImGui::Text("X:"); ImGui::SameLine();  ImGui::PushItemWidth(60);
					ImGui::TextColored({ 255, 255, 0, 255 }, "%f", trans->rotation.x); ImGui::SameLine();
					ImGui::Text("Y:"); ImGui::SameLine();
					ImGui::TextColored({ 255, 255, 0, 255 }, "%f", trans->rotation.y); ImGui::SameLine();
					ImGui::Text("Z:"); ImGui::SameLine();
					ImGui::TextColored({ 255, 255, 0, 255 }, "%f", trans->rotation.z);

					ImGui::Text("Size"); ImGui::SameLine();
					ImGui::Text("X:"); ImGui::SameLine();  ImGui::PushItemWidth(60);
					ImGui::TextColored({ 255, 255, 0, 255 }, "%f", trans->scale.x); ImGui::SameLine();
					ImGui::Text("Y:"); ImGui::SameLine();
					ImGui::TextColored({ 255, 255, 0, 255 }, "%f", trans->scale.y); ImGui::SameLine();
					ImGui::Text("Z:"); ImGui::SameLine();
					ImGui::TextColored({ 255, 255, 0, 255 }, "%f", trans->scale.z);
			}
		}
		if (mesh != nullptr)
		{
			if (ImGui::CollapsingHeader("Mesh"))
			{
			
					ImGui::Text("Triangle Count:");
					ImGui::SameLine();
					ImGui::TextColored({ 255, 255, 0, 255 }, "%i", mesh->n_indexes / 3);
			}
		}
		if (material != nullptr)
		{
			if (ImGui::CollapsingHeader("Texture"))
			{
			
					ImGui::Text("Texture Size:");
					ImGui::SameLine();
					ImGui::TextColored({ 255, 255, 0, 255 },"%i * %i", material->width, material->height);
					ImGui::Text("Texture Path:");
					ImGui::SameLine();
					ImGui::TextColored({ 255, 255, 0, 255 }, ("%s", material->path.c_str()));

					ImGui::Image((void*)(intptr_t)material->id_texture, ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));


					ImGui::Checkbox("Checker texture", &mesh->checkered);
			}
		}
	}

	ImGui::End();

	return true;
}
