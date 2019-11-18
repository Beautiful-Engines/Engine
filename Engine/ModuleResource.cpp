#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleImport.h"
#include "ModuleResource.h"

#include "ImportMesh.h"
#include "ResourceMesh.h"
#include "ImportTexture.h"
#include "ResourceTexture.h"
#include "ImportModel.h"
#include "ResourceModel.h"

#include <fstream>

ModuleResource::ModuleResource(bool start_enabled) : Module(start_enabled)
{
	name = "Resource";
}

ModuleResource::~ModuleResource()
{
}

bool ModuleResource::Start()
{
	LOG("importing all assets");
	LoadAllAssets();

	return true;
}

Resource* ModuleResource::CreateResource(const char* extension, uint UID)
{
	Resource* r = nullptr;

	if (extension == OUR_MESH_EXTENSION)
	{
		r = new ResourceMesh();
	}
	else if (extension == OUR_TEXTURE_EXTENSION)
	{
		r = new ResourceTexture();
	}
	else if (extension == OUR_MODEL_EXTENSION)
	{
		r = new ResourceModel();
	}
	
	if (r != nullptr && UID != 0)
	{
		r->SetId(UID);
	}
	if (r != nullptr)
		resources.emplace(r->GetId(), r);

	return r;
}

Resource* ModuleResource::Get(uint _UID)
{
	std::map<uint, Resource*>::iterator it = resources.find(_UID);
	if (it != resources.end())
		return it->second;

	return nullptr;
}

uint ModuleResource::GetId(std::string _file)
{
	for (std::map<uint, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if (it->second->GetName() == _file)
			return it->first;
	}
		
	return 0;
}

void ModuleResource::LoadAllAssets()
{
	std::vector<std::string> files_temp;
	std::vector<std::string> files;
	std::vector<std::string> directories;
	App->file_system->DiscoverFiles(ASSETS_FOLDER, files_temp, directories);
	for (int i = 0; i < files_temp.size(); ++i)
	{
		if (files_temp[i].find(".meta") > 1000)
			files.push_back(files_temp[i]);
	}

	std::vector<std::string>::iterator iterator = files.begin();

	for (; iterator != files.end(); ++iterator)
	{
		App->file_system->SplitFilePath((*iterator).c_str(), nullptr, nullptr);
		App->importer->ImportFile((ASSETS_FOLDER + (*iterator)).c_str());
	}
	App->importer->import_texture->DefaultTexture();
}

void ModuleResource::LoadFile(const char * _path)
{
	std::string spath = _path;
	std::ifstream ifstream(spath);
	nlohmann::json json = nlohmann::json::parse(ifstream);

	uint UID = json["id"];
	std::string exported_file = json["exported_file"];
	std::string name = json["name"];
	std::string extension;
	App->file_system->SplitFilePath(exported_file.c_str(), nullptr, nullptr, &extension);
	Resource *resource = nullptr;
	if ("." + extension == OUR_MESH_EXTENSION)
	{
		resource = CreateResource(OUR_MESH_EXTENSION, UID);
		resource->SetFile(exported_file);
		resource->SetName(name);
		App->importer->import_mesh->LoadMeshFromResource((ResourceMesh*)resource);
		
	}
	else if ("." + extension == OUR_TEXTURE_EXTENSION)
	{
		resource = CreateResource(OUR_TEXTURE_EXTENSION, UID);
		resource->SetName(name);
		App->importer->import_texture->LoadTexture(exported_file.c_str(), nullptr, (ResourceTexture*)resource);
	}
	else if ("." + extension == OUR_MODEL_EXTENSION)
	{
		resource = CreateResource(OUR_MODEL_EXTENSION, UID);
		resource->SetFile(exported_file);
		resource->SetName(name);
		App->importer->import_model->LoadModel((ResourceModel*)resource);
	}
}
