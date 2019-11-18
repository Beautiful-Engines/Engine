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
		if (it->second->GetName() == _file)
			return it->first;

	return 0;
}

void ModuleResource::LoadAllAssets()
{
	std::vector<std::string> files;
	std::vector<std::string> directories;
	std::string extension;
	App->file_system->DiscoverFiles(ASSETS_FOLDER, files, directories);

	std::vector<std::string>::iterator iterator = files.begin();

	for (; iterator != files.end(); ++iterator)
	{
		App->file_system->SplitFilePath((*iterator).c_str(), nullptr, nullptr, &extension);
		App->importer->ImportFile((ASSETS_FOLDER + (*iterator)).c_str());
	}
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
		App->importer->import_mesh->LoadMeshFromResource((ResourceMesh*)resource);
		resource->SetFile(exported_file);
		resource->SetName(name);
	}
	else if ("." + extension == OUR_TEXTURE_EXTENSION)
	{
		resource = CreateResource(OUR_TEXTURE_EXTENSION, UID);
		App->importer->import_texture->LoadTexture(exported_file.c_str(), nullptr, (ResourceTexture*)resource);
		resource->SetName(name);
	}
	else if ("." + extension == OUR_MODEL_EXTENSION)
	{
		Resource* resource = CreateResource(OUR_MODEL_EXTENSION);
		resource->SetFile(_path);
		/*App->importer->import_model->LoadModel();*/
	}
		
}
