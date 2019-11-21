#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentCamera.h"
#include "GameObject.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
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
	ComponentTexture *texture = nullptr;
	ComponentCamera *camera = nullptr;
	ResourceTexture* resource_texture = nullptr;

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
			if (go->GetComponents()[i]->GetType() == ComponentType::CAMERA)
			{
				camera = (ComponentCamera*)go->GetComponents()[i];
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
			if (ImGui::Button("Delete", { 100,20 }))
				App->scene->DeleteGameObject(go);
			ImGui::Checkbox("Hide", &go->hide);
			ImGui::Checkbox("Static", &go->is_static);
			if (go->is_static)
			{
				go->SetStatic(true);
				App->scene->CreateQuadtree();
			}
			else
			{
				go->SetStatic(false);
				App->scene->CreateQuadtree();
			}

			if (go->hide)
				go->Disable();
			else
				go->Enable();

			if (ImGui::CollapsingHeader("Transform"))
			{
				float* p = (float*)&trans->local_position;
				if (ImGui::InputFloat3("Position", p, 2))
				{
					go->GetTransform()->SetLocalPosition((float3)p);
				}
				

				float* r = (float*)&go->GetTransform()->GetLocalRotationToEuler();
				if(ImGui::InputFloat3("Rotation", r, 2)) {
					go->GetTransform()->SetLocalRotationFromEuler((float3)r);
				};

				float* s = (float*)&trans->local_scale;
				if (ImGui::InputFloat3("Scale", s, 2))
				{
					go->GetTransform()->SetLocalScale((float3)s);
				}
			
				//go->GetTransform()->GetTransformMatrix();
				/*ImGui::Text("Position"); ImGui::SameLine();
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
				ImGui::TextColored({ 255, 255, 0, 255 }, "%f", trans->scale.z);*/
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
				//if (mesh->debug_bb)
				//	go->UpdateBB();
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
