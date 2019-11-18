#include "Application.h"
#include "ModuleScene.h"
#include "ModuleFileSystem.h"
#include "ModuleResource.h"

#include "GameObject.h"
#include "ComponentTexture.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"

#include "DeviL/include/il.h"
#include "DeviL/include/ilu.h"
#include "DeviL/include/ilut.h"

#pragma comment (lib, "DeviL/lib/DevIL.lib")
#pragma comment (lib, "DeviL/lib/ILU.lib")
#pragma comment (lib, "DeviL/lib/ILUT.lib")

#include "ImportTexture.h"

#include <fstream>
#include <iomanip>

ImportTexture::ImportTexture()
{
}

ImportTexture::~ImportTexture()
{
}

bool ImportTexture::Init()
{
	ilInit();
	iluInit();
	ilutInit();
	ilEnable(IL_CONV_PAL);
	ilutEnable(ILUT_OPENGL_CONV);
	ilutRenderer(ILUT_OPENGL);

	LOG("Initialized DevIL version: %i", ilGetInteger(IL_VERSION_NUM));

	return true;
}

bool ImportTexture::CleanUp()
{
	return true;
}

bool ImportTexture::Import(const char* _import_file, std::string& _output_file)
{
	uint image_id;

	ilGenImages(1, &image_id); // Grab a new image name.
	ilBindImage(image_id);
	ilLoadImage(_import_file);

	// Name
	std::string name_path = _import_file;
	uint pos = name_path.find_last_of("\\/");
	name_path = (name_path.substr(pos + 1)).c_str();
	pos = name_path.find(".");
	std::string name_object = name_path.substr(0, pos);

	ILuint size;
	ILubyte *data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // To pick a specific DXT compression use 
	size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer 
	if (size > 0) {
		data = new ILubyte[size]; // allocate data buffer   
		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function        
		{
			_output_file = LIBRARY_TEXTURES_FOLDER + name_object + ".dds";
			App->file_system->Save(_output_file.c_str(), data, size);
			nlohmann::json json = {
				{ "exported_file", _output_file },
				{ "name", _import_file },
				{ "id", LoadTexture(_output_file.c_str()) }
			};
			// writting to .meta
			std::string meta_path = _import_file;
			meta_path += ".meta";
			std::ofstream ofstream(meta_path);
			ofstream << std::setw(4) << json << std::endl;
		}
		RELEASE_ARRAY(data);
	}
	ilDeleteImages(1, &image_id);

	return true;
}

uint ImportTexture::LoadTexture(const char* _path, ResourceMesh* resource_mesh, ResourceTexture* _resource_texture)
{
	// Name
	std::string name_path = _path;
	uint pos = name_path.find_last_of("\\/");
	name_path = (name_path.substr(pos + 1)).c_str();

	ResourceTexture* resource_texture = _resource_texture;

	if (resource_mesh != nullptr)
	{
		uint id_tex;
		ilGenImages(1, &id_tex);
		ilBindImage(id_tex);
		
		if (ilLoadImage(_path))
		{
			if(resource_texture == nullptr)
				resource_texture = (ResourceTexture*)App->resource->CreateResource(OUR_TEXTURE_EXTENSION);

			resource_texture->id_texture = ilutGLBindTexImage();
			resource_texture->path = _path;
			resource_texture->SetFile(_path);
			resource_texture->width = ilGetInteger(IL_IMAGE_WIDTH);
			resource_texture->height = ilGetInteger(IL_IMAGE_HEIGHT);
			resource_mesh->id_texture = resource_texture->id_texture;

			LOG("Added %s to %s", name_path.c_str(), resource_mesh->GetName());
		}
		else
		{
			auto error = ilGetError();
			LOG("Error loading texture %s. Error: %s", name_path.c_str(), ilGetString(error));
		}

		ilDeleteImages(1, &id_tex);
	}
	else if(App->scene->GetSelected() != nullptr)
	{
		if (App->scene->GetSelected()->GetChildren().size() > 0)
		{
			std::vector<GameObject*> children_game_objects = App->scene->GetSelected()->GetChildren();

			for (uint j = 0; j < children_game_objects.size(); ++j)
			{
				LoadTexture(name_path.c_str(), children_game_objects[j]->GetMesh()->GetResourceMesh());
			}
		}
		else
		{
			LoadTexture(name_path.c_str(), App->scene->GetSelected()->GetMesh()->GetResourceMesh());
		}
	}
	else
	{
		uint id_tex;
		ilGenImages(1, &id_tex);
		ilBindImage(id_tex);

		if (ilLoadImage(_path))
		{
			if (resource_texture == nullptr)
				resource_texture = (ResourceTexture*)App->resource->CreateResource(OUR_TEXTURE_EXTENSION);

			resource_texture->id_texture = ilutGLBindTexImage();
			resource_texture->path = _path;
			resource_texture->SetFile(_path);
			resource_texture->width = ilGetInteger(IL_IMAGE_WIDTH);
			resource_texture->height = ilGetInteger(IL_IMAGE_HEIGHT);

			LOG("Added %s to Engine", name_path.c_str());
		}
		else
		{
			auto error = ilGetError();
			LOG("Error loading texture %s. Error: %s", name_path.c_str(), ilGetString(error));
		}

		ilDeleteImages(1, &id_tex);
	}

	return resource_texture->GetId();
}

void ImportTexture::DefaultTexture(ResourceMesh* go_texturedefault)
{
	ResourceTexture *resource_texture = new ResourceTexture();

	resource_texture->path = "DefaultTexture";
	resource_texture->width = 128;
	resource_texture->height = 128;
	GLubyte checkImage[128][128][4];
	for (int i = 0; i < 128; i++)
	{
		for (int j = 0; j < 128; j++)
		{
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &resource_texture->id_texture);
	glBindTexture(GL_TEXTURE_2D, resource_texture->id_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	go_texturedefault->id_default_texture = resource_texture->id_texture;

}