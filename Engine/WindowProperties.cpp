#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentCamera.h"
#include "ComponentAnimation.h"
#include "ComponentBone.h"
#include "GameObject.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceAnimation.h"
#include "ResourceBone.h"
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
	go = nullptr;
	trans = nullptr;
	mesh = nullptr;
	texture = nullptr;
	camera = nullptr;
	resource_texture = nullptr;

	go = App->scene->GetSelected();

	if (go != nullptr)
	{
		for (uint i = 0; i < go->GetComponents().size(); ++i)
		{
			if (go->GetComponents()[i]->GetType() == ComponentType::MESH)
			{
				mesh = (ComponentMesh*)go->GetComponents()[i];
			}
			if (go->GetComponents()[i]->GetType() == ComponentType::TEXTURE)
			{
				texture = (ComponentTexture*)go->GetComponents()[i];
				if(!texture->checkered && texture->texture != nullptr)
					resource_texture = texture->texture;
				else
					resource_texture = texture->default_texture;
			}
			if (go->GetComponents()[i]->GetType() == ComponentType::TRANSFORM)
			{
				trans = (ComponentTransform*)go->GetComponents()[i];
			}
			if (go->GetComponents()[i]->GetType() == ComponentType::ANIMATION)
			{
				animations_temp.push_back((ComponentAnimation*)go->GetComponents()[i]);
			}
			if (go->GetComponents()[i]->GetType() == ComponentType::BONE)
			{
				bones_temp.push_back((ComponentBone*)go->GetComponents()[i]);
			}
			if (go->GetComponents()[i]->GetType() == ComponentType::CAMERA)
			{
				camera = (ComponentCamera*)go->GetComponents()[i];
			}
		}	
		animations = animations_temp;
		animations_temp.clear();
		bones = bones_temp;
		bones_temp.clear();
	}

	ImGuiWindowFlags aboutFlags = 0;
	aboutFlags |= ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin("Properties", &enabled, aboutFlags))
	{
		ImGui::SetWindowFontScale(1);

		if (trans != nullptr)
		{
			/*if (ImGui::Button("Delete", { 100,20 }))
				App->scene->DeleteGameObject(go);*/
			if (ImGui::Checkbox("Hide", &go->hide))
			{
				if (go->hide)
					go->Disable();
				else
					go->Enable();
			}
			if (ImGui::Checkbox("Static", &go->is_static))
			{
				go->SetStatic(go->is_static);
				App->scene->CreateQuadtree();
			}
				

			if (ImGui::CollapsingHeader("Transform"))
			{
				float* p = (float*)&trans->local_position;
				if (!go->is_static)
				{
					if (ImGui::InputFloat3("Position", p, 2))
					{
						go->GetTransform()->SetLocalPosition((float3)p);
					}
				}
				else
					ImGui::InputFloat3("Position", p, 2, ImGuiInputTextFlags_ReadOnly);
				

				float* r = (float*)&go->GetTransform()->GetLocalRotationToEuler();
				if (!go->is_static)
				{
					if(ImGui::InputFloat3("Rotation", r, 2) && !go->is_static) 
					{
						go->GetTransform()->SetLocalRotationFromEuler((float3)r);
					}
				}
				else
					ImGui::InputFloat3("Rotation", r, 2, ImGuiInputTextFlags_ReadOnly);

				float* s = (float*)&trans->local_scale;
				if (!go->is_static)
				{
					if (ImGui::InputFloat3("Scale", s, 2) && !go->is_static)
					{
						go->GetTransform()->SetLocalScale((float3)s);
					}
				}
				else
					ImGui::InputFloat3("Scale", s, 2, ImGuiInputTextFlags_ReadOnly);
			}
		}
		if (mesh != nullptr)
		{
			if (ImGui::CollapsingHeader("Mesh"))
			{
				ImGui::Text("Vertex Count:");
				ImGui::SameLine();
				ImGui::TextColored({ 255, 255, 0, 255 }, "%i", mesh->GetResourceMesh()->n_vertices / 3);

				ImGui::Text("Triangle Count:");
				ImGui::SameLine();
				ImGui::TextColored({ 255, 255, 0, 255 }, "%i", mesh->GetResourceMesh()->n_indexes / 3);

				ImGui::Checkbox("Vertex Normals", &mesh->vertex_normals);
				ImGui::Checkbox("Face Normals", &mesh->face_normals);
				ImGui::Checkbox("Bounding Box", &mesh->debug_bb);
			}
		}
		if (texture != nullptr)
		{
			if (ImGui::CollapsingHeader("Texture"))
			{
				ImGui::Checkbox("Active", &mesh->textures);

				ImGui::Text("Texture Size:");
				ImGui::SameLine();
				ImGui::TextColored({ 255, 255, 0, 255 }, "%i * %i", resource_texture->width, resource_texture->height);
				ImGui::Text("Texture Path:");
				ImGui::SameLine();
				ImGui::TextColored({ 255, 255, 0, 255 }, ("%s", resource_texture->path.c_str()));

				ImGui::Image((void*)(intptr_t)resource_texture->id_texture, ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
				
				ImGui::Checkbox("Checker texture", &texture->checkered);
			}
		}
		if (animations.size() > 0)
		{
			if (ImGui::CollapsingHeader("Animation"))
			{
				std::vector<ComponentAnimation*>::iterator iterator_animation = animations.begin();

				for (; iterator_animation != animations.end(); ++iterator_animation)
				{
					ImGui::Text("Animation Name:");
					ImGui::SameLine();
					ImGui::TextColored({ 255, 255, 0, 255 }, "%s", (*iterator_animation)->resource_animation->name_anim.c_str());
					ImGui::Text("Animation Duration:");
					ImGui::SameLine();
					ImGui::TextColored({ 255, 255, 0, 255 }, "%i", (*iterator_animation)->resource_animation->duration);
					ImGui::Text("Animation Ticks Per Second:");
					ImGui::SameLine();
					ImGui::TextColored({ 255, 255, 0, 255 }, "%i", (*iterator_animation)->resource_animation->ticks_per_second);
				}
			}
		}
		if (bones.size() > 0)
		{
			if (ImGui::CollapsingHeader("Bones"))
			{
				std::vector<ComponentBone*>::iterator iterator_bone = bones.begin();

				for (; iterator_bone != bones.end(); ++iterator_bone)
				{
					ImGui::Text("Bone Name:");
					ImGui::SameLine();
					ImGui::TextColored({ 255, 255, 0, 255 }, "%s", (*iterator_bone)->resource_bone->name_bone.c_str());
				}
			}
		}
		if (camera != nullptr)
		{
			if (ImGui::CollapsingHeader("Camera"))
			{
				ImGui::Checkbox("Main Camera", &camera->main_camera);
				ImGui::Checkbox("Frustum Culling", &camera->frustum_culling);

				float npd = camera->GetNearPlaneDistance();
				if (ImGui::SliderFloat("Near plane", &npd, 0.1f, 1.0f, "%.2f")) {
					camera->SetNearPlaneDistance(npd);
				}

				float fpd = camera->GetFarPlaneDistance();
				if (ImGui::SliderFloat("Far plane", &fpd, 1.f, 500.f, "%.2f")) {
					camera->SetFarPlaneDistance(fpd);
				}

				float fov = camera->GetVerticalFOV();
				if (ImGui::SliderAngle("FOV", &fov, 20.0f, 90.0f)) {
					camera->SetFOV(fov);
				}
			}
		}
	}

	ImGui::End();

	return true;
}
