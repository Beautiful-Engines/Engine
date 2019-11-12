#include "WindowScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
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
	int image_w, image_h;
	GetSizeWithAspectRatio(App->window->GetWindowWidth(), App->window->GetWindowHeight(), w, h, image_w, image_h);
	ImGui::GetWindowDrawList()->AddImage(
		(void *)App->renderer3D->scene_texture_id,
		screen_pos,
		ImVec2(screen_pos.x + w, screen_pos.y + h),
		ImVec2(0, 1),
		ImVec2(1, 0));
	ImGui::End();

	return true;
}

void WindowScene::Start()
{
}

bool WindowScene::IsInside(const float2 & pos) const
{
	return false;
}

void WindowScene::GetSizeWithAspectRatio(int current_width, int current_height, int wanted_width, int wanted_height, int & new_width, int & new_height)
{
	float scale_height = (float)wanted_height / (float)current_height;
	float scale_width = (float)wanted_width / (float)current_width;

	float scale = (std::min)(scale_height, scale_width);

	new_width = current_width * scale;
	new_height = current_height * scale;
}
