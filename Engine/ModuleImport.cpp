#include "Application.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "ModuleScene.h"
#include "ModuleImport.h"
#include "ModuleFileSystem.h"
#include "ModuleResource.h"

#include "ImportModel.h"
#include "ImportMesh.h"
#include "ImportTexture.h"
#include "ImportAnimation.h"

#include <fstream>

ModuleImport::ModuleImport(bool start_enabled) : Module(start_enabled)
{
	name = "Import";
}

ModuleImport::~ModuleImport()
{
}

bool ModuleImport::Init()
{
	import_model = new ImportModel();
	import_model->Init();

	import_mesh = new ImportMesh();
	import_mesh->Init();

	import_texture = new ImportTexture();
	import_texture->Init();

	return true;
}
bool ModuleImport::Start()
{
	return true;
}

bool ModuleImport::CleanUp()
{
	return true;
}

void ModuleImport::ImportFile(const char* _path, bool _change_meta, bool _final_path)
{

	//copy file
	std::string normalized_path = _path;
	App->file_system->NormalizePath(normalized_path);
	std::string file, extension;
	App->file_system->SplitFilePath(normalized_path.c_str(), nullptr, &file, &extension);
	std::string final_path = "";
	if(!_final_path)
		final_path = ASSETS_FOLDER + file;
	else
		final_path = _path;

	std::string meta_path = final_path + ".meta";

	if (!App->file_system->Exists(final_path.c_str()))
	{
		// Copy to assets folder
		App->file_system->CopyFromOutsideFS(normalized_path.c_str(), final_path.c_str());
	}
	if (!App->file_system->Exists(meta_path.c_str()) || _change_meta)
	{
		// Importing
		if (extension == "fbx")
		{
			// Import

			import_model->ImportFBX(final_path.c_str());
		}
		else if (extension == "png" || extension == "dds" || extension == "tga")
		{
			// Import
			import_texture->Import(final_path.c_str());
		}
	}
	if(App->file_system->Exists(meta_path.c_str()))
	{
		// Use meta file
		std::ifstream ifstream(meta_path);
		nlohmann::json json = nlohmann::json::parse(ifstream);
		std::string exported_file = json["exported_file"];

		if (App->file_system->Exists(exported_file.c_str()))
			App->resource->LoadFile(meta_path.c_str());
		else
		{
			ImportFile(_path, true, true);
		}

	}
}

