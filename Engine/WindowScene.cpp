#include "WindowScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ComponentCamera.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "ModuleResource.h"
#include "ComponentTransform.h"
#include "ImGui/imgui.h"
#include "ResourceModel.h"
#include "MathGeoLib/include/Math/float4x4.h"

#include "ImGui/imgui_stdlib.h"
#include "imgui/imgui_internal.h"

//tbch
WindowScene::WindowScene()
{
	enabled = true;
	guizmo_mode = ImGuizmo::MODE::LOCAL;
	guizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
}


WindowScene::~WindowScene()
{
}

bool WindowScene::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ImGui::Begin("Scene", &enabled, ImGuiWindowFlags_MenuBar);
	w = ImGui::GetWindowWidth();
	h = ImGui::GetWindowHeight();

	screen_pos = ImGui::GetCursorScreenPos();
	GetSizeWithAspectRatio(App->window->GetWindowWidth(), App->window->GetWindowHeight());
	ImGui::GetWindowDrawList()->AddImage(
		(void *)App->renderer3D->scene_texture_id,
		screen_pos,
		ImVec2(screen_pos.x + w, screen_pos.y + h),
		ImVec2(0, 1),
		ImVec2(1, 0));

	// Get Item from WindowProject
	if (ImGui::BeginDragDropTargetCustom(ImGui::GetCurrentWindow()->Rect(), (ImGuiID)"Scene"))
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
		{
			uint* id = (uint*)payload->Data;
			if(App->resource->Get(*id)->GetType() == Resource::RESOURCE_TYPE::RESOURCE_MODEL)
				App->scene->CreateGameObjectModel((ResourceModel*)App->resource->Get(*id));
		}
		ImGui::EndDragDropTarget();
	}

	if (App->scene->GetSelected()) {
		TransformInputs();
		DrawGuizmo();
	}
	ImGui::End();
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		float2 mousePos = { ((float)App->input->GetMouseX()-(screen_pos.x+w/2)), ((float)App->input->GetMouseY()-(screen_pos.y+h/2)) };
		mousePos = { mousePos.x / w * 2, mousePos.y / h * 2 };
		App->camera->OnClick(mousePos);
	}
	return true;
}

void WindowScene::Start()
{
}

bool WindowScene::IsInside(const float2 & pos) const
{
	return false;
}

void WindowScene::GetSizeWithAspectRatio(int current_width, int current_height)
{
	float scale_height = (float)h / (float)current_height;
	float scale_width = (float)w / (float)current_width;

	float scale = (std::min)(scale_height, scale_width);

	w = current_width * scale;
	h = current_height * scale;
}

void WindowScene::TransformInputs()
{
	if (!ImGuizmo::IsUsing())
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			guizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			guizmo_operation = ImGuizmo::OPERATION::ROTATE;
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
			guizmo_operation = ImGuizmo::OPERATION::SCALE;
	}
}

void WindowScene::DrawGuizmo()
{
	ImGuizmo::Enable(true);
	float4x4 delta, transform, view, projection;

	transform = App->scene->GetSelected()->GetTransform()->transform_matrix.Transposed();
	projection = App->renderer3D->camera->GetOpenGLProjectionMatrix();
	view = App->renderer3D->camera->GetOpenGLViewMatrix();

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(float(ImGui::GetCursorScreenPos().x), float(ImGui::GetCursorScreenPos().y), float(w), float(h));
	ImGuizmo::SetDrawlist();
	ImGuizmo::Manipulate((const float*)&view,(const float*)&projection,guizmo_operation,guizmo_mode,(float*)&transform,(float*)&delta);

	if (ImGuizmo::IsUsing())
	{
		App->scene->GetSelected()->GetTransform()->SetTransformMatrix(transform.Transposed());
	}
}
