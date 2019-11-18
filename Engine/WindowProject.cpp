#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleResource.h"
#include "ModuleScene.h"
#include "ModuleImport.h"
#include "ImportTexture.h"
#include "ResourceTexture.h"

#include "WindowProject.h"


WindowProject::WindowProject() : WindowEngine()
{
	enabled = true;
	columns = 8;
	image_size = 120;
	spacing = 20;
	offset = 10;
}


WindowProject::~WindowProject()
{
}

bool WindowProject::Draw()
{
	ImGui::Begin("Project", &enabled);

	ImGui::Text("Assets");
	ImGui::Separator();

	ImVec2 pos = ImGui::GetCursorPos();

	std::vector<std::string> files_temp;
	std::vector<std::string> files;
	std::vector<std::string> directories;
	App->file_system->DiscoverFiles(ASSETS_FOLDER, files_temp, directories);
	if (!files_temp.empty())
	{
		for (int i = 0; i < files_temp.size(); ++i)
		{
			if (files_temp[i].find(".meta") > 1000)
				files.push_back(files_temp[i]);
		}
	}
	
	int line = 0;
	for (int i = 0; i < directories.size(); ++i)
	{
		ImGui::PushID(i);

		ImGui::SetCursorPosX(pos.x + (i - (line * columns)) * (image_size + spacing) + offset);
		ImGui::SetCursorPosY(pos.y + line * (image_size + spacing) + offset);

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			uint id = (uint)App->resource->GetId(ASSETS_FOLDER + directories[i]);
			ImGui::SetDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F, &id, sizeof(uint));
			ImGui::EndDragDropSource();
		}

		ImGui::SetCursorPosX(pos.x + (i - (line * columns)) * (image_size + spacing) + offset);
		ImGui::SetCursorPosY(pos.y + line * (image_size + spacing) + image_size + offset + offset);

		std::string text = FitTextToImage(directories[i]);
		ImGui::Text(text.c_str());

		if ((i + 1) % columns == 0)
			line++;

		ImGui::PopID();
	}
	for (int i = 0; i < files.size(); ++i)
	{
		ImGui::PushID(i);

		ImGui::SetCursorPosX(pos.x + (i - (line * columns)) * (image_size + spacing) + offset);
		ImGui::SetCursorPosY(pos.y + line * (image_size + spacing) + offset);

		if (App->resource->Get(App->resource->GetId(ASSETS_FOLDER + files[i])) != nullptr)
		{
			if (App->resource->Get(App->resource->GetId(ASSETS_FOLDER + files[i]))->GetType() == Resource::RESOURCE_TYPE::RESOURCE_TEXTURE)
			{
				ImGui::Image((void*)(intptr_t)((ResourceTexture*)App->resource->Get(App->resource->GetId(ASSETS_FOLDER + files[i])))->id_texture,
					ImVec2(image_size, image_size), ImVec2(0, 1), ImVec2(1, 0));
			}
			else
			{
				ImGui::Image((void*)(intptr_t)((ResourceTexture*)App->resource->Get(App->resource->GetId("DefaultTexture")))->id_texture,
					ImVec2(image_size, image_size), ImVec2(0, 1), ImVec2(1, 0));
			}
		}


		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			uint id = App->resource->GetId(ASSETS_FOLDER + files[i]);
			ImGui::SetDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F, &id, sizeof(uint));
			ImGui::EndDragDropSource();
		}

		ImGui::SetCursorPosX(pos.x + (i - (line * columns)) * (image_size + spacing) + offset);
		ImGui::SetCursorPosY(pos.y + line * (image_size + spacing) + image_size + offset + offset);

		std::string text = FitTextToImage(files[i]);
		ImGui::Text(text.c_str());

		if ((i + 1) % columns == 0)
			line++;

		ImGui::PopID();
		
	}
	
	ImGui::End();

	return true;
}


std::string WindowProject::FitTextToImage(std::string text)
{
	std::string fitted_text = text;
	uint length = text.size();

	uint text_size = ImGui::CalcTextSize(text.c_str()).x;
	

	if (text_size > image_size)
	{
		uint char_size = text_size / length;
		uint dots_size = ImGui::CalcTextSize("...").x;
		uint new_length = ((image_size - dots_size) / char_size);
		fitted_text = fitted_text.substr(0, new_length);
		fitted_text.append("...");
	}

	return fitted_text;
}