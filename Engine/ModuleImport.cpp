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

void ModuleImport::ImportFile(const char* _path)
{

	//copy file
	std::string normalized_path = _path;
	App->file_system->NormalizePath(normalized_path);
	std::string file, extension;
	App->file_system->SplitFilePath(normalized_path.c_str(), nullptr, &file, &extension);

	std::string final_path = ASSETS_FOLDER + file;
	std::string meta_path = final_path + ".meta";

	if (!App->file_system->Exists(final_path.c_str()))
	{
		// Copy to assets folder
		App->file_system->CopyFromOutsideFS(normalized_path.c_str(), final_path.c_str());
	}
	if (!App->file_system->Exists(meta_path.c_str()))
	{
		// Importing
		if (extension == "fbx")
		{
			// Import
			import_model->ImportFBX(final_path.c_str());
		}
		else if (extension == "png" || extension == "dds")
		{
			// Import
			std::string output_file;
			import_texture->Import(final_path.c_str(), output_file);
		}
	}
	if(App->file_system->Exists(meta_path.c_str()))
	{
		// Use meta file
		App->resource->LoadFile(meta_path.c_str());
	}
}

