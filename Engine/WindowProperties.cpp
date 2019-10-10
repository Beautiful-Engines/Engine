#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"
#include "WindowProperties.h"

#include "ImGui\imgui_stdlib.h"


WindowProperties::WindowProperties() : WindowEngine()
{
}


WindowProperties::~WindowProperties()
{
}

bool WindowProperties::Draw()
{
	ImGuiWindowFlags aboutFlags = 0;
	aboutFlags |= ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin("Properties", &enabled, aboutFlags))
	{
		ImGui::SetWindowFontScale(1);
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Position"); ImGui::SameLine();
			ImGui::Text("X:"); ImGui::SameLine();  ImGui::PushItemWidth(60);
			ImGui::InputFloat("", new float(0.0f)); ImGui::SameLine();
			ImGui::Text("Y:"); ImGui::SameLine();
			ImGui::InputFloat("", new float(0.0f)); ImGui::SameLine();
			ImGui::Text("Z:"); ImGui::SameLine();
			ImGui::InputFloat("", new float(0.0f));

			ImGui::Text("Rotation"); ImGui::SameLine();
			ImGui::Text("X:"); ImGui::SameLine();  ImGui::PushItemWidth(60);
			ImGui::InputFloat("", new float(0.0f)); ImGui::SameLine();
			ImGui::Text("Y:"); ImGui::SameLine();
			ImGui::InputFloat("", new float(0.0f)); ImGui::SameLine();
			ImGui::Text("Z:"); ImGui::SameLine();
			ImGui::InputFloat("", new float(0.0f));

			ImGui::Text("Size"); ImGui::SameLine();
			ImGui::Text("X:"); ImGui::SameLine();  ImGui::PushItemWidth(60);
			ImGui::InputFloat("", new float(1.0f)); ImGui::SameLine();
			ImGui::Text("Y:"); ImGui::SameLine();
			ImGui::InputFloat("", new float(1.0f)); ImGui::SameLine();
			ImGui::Text("Z:"); ImGui::SameLine();
			ImGui::InputFloat("", new float(1.0f));
		}
		if (ImGui::CollapsingHeader("Mesh"))
		{
			ImGui::Text("Triangle Count:");
			ImGui::SameLine();
			ImGui::TextColored({ 255, 255, 0, 255 }, "%i", 1350);
		}
		if (ImGui::CollapsingHeader("Texture"))
		{
			ImGui::Text("Texture Size:");
			ImGui::SameLine();
			ImGui::TextColored({ 255, 255, 0, 255 }, "%i", 720);
		}
	}

	ImGui::End();

	return true;
}
