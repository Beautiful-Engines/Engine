#include "WindowScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"

//tbch
WindowScene::WindowScene()
{
	enabled = true;
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
