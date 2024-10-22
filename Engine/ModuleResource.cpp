#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleImport.h"
#include "ModuleResource.h"
#include "ModuleScene.h"
#include "ImportMesh.h"
#include "ResourceMesh.h"
#include "ImportTexture.h"
#include "ResourceTexture.h"
#include "ImportModel.h"
#include "ResourceModel.h"
#include "ImportAnimation.h"
#include "ResourceAnimation.h"
#include "ResourceBone.h"
#include "Primitive.h"

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
	else if (extension == OUR_ANIMATION_EXTENSION)
	{
		r = new ResourceAnimation();
	}
	else if (extension == OUR_BONE_EXTENSION)
	{
		r = new ResourceBone();
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

Resource* ModuleResource::GetAndUse(uint _UID)
{
	std::map<uint, Resource*>::iterator it = resources.find(_UID);
	if (it != resources.end())
	{
		it->second->SetCantities(1);
		return it->second;
	}

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

std::map<uint, Resource*> ModuleResource::GetResources()
{
	return resources;
}

void ModuleResource::LoadAllAssets()
{
	App->importer->import_texture->DefaultTexture();

	LoadByFolder(ASSETS_FOLDER);

	// just for delivery, then delete it
	std::string path = "skeleton@idle.fbx";
	path = ASSETS_FOLDER + path;
	GameObject* go = App->importer->import_model->CreateModel((ResourceModel*)App->resource->GetAndUse(App->resource->GetId(path.c_str())));
	go->SetStatic(false);
	go->GetTransform()->SetLocalScale({ 0.3, 0.3, 0.3 });
	go->SetStatic(true);

	path = "skeleton@attack.fbx";
	path = ASSETS_FOLDER + path;
	go = App->importer->import_model->CreateModel((ResourceModel*)App->resource->GetAndUse(App->resource->GetId(path.c_str())));
	
	path = "skeleton@run.fbx";
	path = ASSETS_FOLDER + path;
	go = App->importer->import_model->CreateModel((ResourceModel*)App->resource->GetAndUse(App->resource->GetId(path.c_str())));
	
	path = "street environment_v01.fbx";
	path = ASSETS_FOLDER + path;
	App->importer->import_model->CreateModel((ResourceModel*)App->resource->GetAndUse(App->resource->GetId(path.c_str())));

}

void ModuleResource::LoadByFolder(const char* _folder)
{
	std::vector<std::string> files_temp;
	std::vector<std::string> files;
	std::vector<std::string> directories;
	App->file_system->DiscoverFiles(_folder, files_temp, directories);
	for (int i = 0; i < files_temp.size(); ++i)
	{
		if (files_temp[i].find(".meta") > 1000)
			files.push_back(files_temp[i]);
	}

	std::vector<std::string>::iterator iterator = files.begin();

	for (; iterator != files.end(); ++iterator)
	{
		if (first_textures && (*iterator).find(".fbx") > 1000)
		{
			App->file_system->SplitFilePath((*iterator).c_str(), nullptr, nullptr);
			App->importer->ImportFile((_folder + (*iterator)).c_str(), false, true);
		}
		else if (!first_textures && (*iterator).find(".fbx") < 1000)
		{
			App->file_system->SplitFilePath((*iterator).c_str(), nullptr, nullptr);
			App->importer->ImportFile((_folder + (*iterator)).c_str(), false, true);
		}
	}

	std::vector<std::string>::iterator iterator2 = directories.begin();
	for (; iterator2 != directories.end(); ++iterator2)
	{
		std::string folder = _folder + (*iterator2) + "/";
		LoadByFolder(folder.c_str());
	}

	if (first_textures)
	{
		first_textures = false;
		LoadByFolder(ASSETS_FOLDER);
	}
}

void ModuleResource::LoadFile(const char * _path)
{
	if (!App->resource->GetId(_path))
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
			ResourceModel* resource_model = (ResourceModel*)resource;

			for (nlohmann::json::iterator iterator = json.find("animations").value().begin(); iterator != json.find("animations").value().end(); ++iterator)
			{
				resource_model->animations.push_back((*iterator)["id"]);
			}
			App->importer->import_model->LoadModel(resource_model);
		}
		else if ("." + extension == OUR_ANIMATION_EXTENSION)
		{
			resource = CreateResource(OUR_ANIMATION_EXTENSION, UID);
			resource->SetFile(exported_file);
			resource->SetName(name);
			App->importer->import_animation->LoadAnimationFromResource((ResourceAnimation*)resource);
		}
		else if ("." + extension == OUR_BONE_EXTENSION)
		{
			resource = CreateResource(OUR_BONE_EXTENSION, UID);
			resource->SetFile(exported_file);
			resource->SetName(name);
			App->importer->import_animation->LoadBoneFromResource((ResourceBone*)resource);
		}
	}
}
