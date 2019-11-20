#include "Application.h"
#include "ModuleResource.h"
#include "Resource.h"
#include "WindowResources.h"

#include <map>

WindowResources::WindowResources() : WindowEngine()
{
	enabled = true;
}

WindowResources::~WindowResources()
{

}

bool WindowResources::Draw()
{
	ImGui::Begin("Resources", &enabled);

	std::map<uint, Resource*> resources = App->resource->GetResources();

	
	if (ImGui::CollapsingHeader("Textures"))
	{
		for (std::map<uint, Resource*>::const_iterator iterator = resources.begin(); iterator != resources.end(); ++iterator)
		{
			if (iterator->second->GetType() == Resource::RESOURCE_TYPE::RESOURCE_TEXTURE)
			{
				ImGui::Separator();
				ImGui::Text("Resource: %s", iterator->second->GetName());
				ImGui::Text("Library file: %s", iterator->second->GetFile());
				ImGui::Text("ID: %u", iterator->first);
				ImGui::Text("References: %i", iterator->second->GetCantities());
			}
		}
	}
	if (ImGui::CollapsingHeader("Meshes"))
	{
		for (std::map<uint, Resource*>::const_iterator iterator = resources.begin(); iterator != resources.end(); ++iterator)
		{
			if (iterator->second->GetType() == Resource::RESOURCE_TYPE::RESOURCE_MESH)
			{
				ImGui::Separator();
				ImGui::Text("Resource: %s", iterator->second->GetName());
				ImGui::Text("Library file: %s", iterator->second->GetFile());
				ImGui::Text("ID: %u", iterator->first);
				ImGui::Text("References: %i", iterator->second->GetCantities());
			}
		}
	}
	if (ImGui::CollapsingHeader("Models"))
	{
		for (std::map<uint, Resource*>::const_iterator iterator = resources.begin(); iterator != resources.end(); ++iterator)
		{
			if (iterator->second->GetType() == Resource::RESOURCE_TYPE::RESOURCE_MODEL)
			{
				ImGui::Separator();
				ImGui::Text("Resource: %s", iterator->second->GetName());
				ImGui::Text("Library file: %s", iterator->second->GetFile());
				ImGui::Text("ID: %u", iterator->first);
				ImGui::Text("References: %i", iterator->second->GetCantities());
			}
		}
	}
	
	ImGui::End();

	return true;
}