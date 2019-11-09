#include "Application.h"
#include "ModuleScene.h"
#include "ModuleFileSystem.h"

#include "GameObject.h"
#include "ComponentMaterial.h"

#include "DeviL/include/il.h"
#include "DeviL/include/ilu.h"
#include "DeviL/include/ilut.h"

#pragma comment (lib, "DeviL/lib/DevIL.lib")
#pragma comment (lib, "DeviL/lib/ILU.lib")
#pragma comment (lib, "DeviL/lib/ILUT.lib")

#include "ImportTexture.h"

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
		}
		RELEASE_ARRAY(data);
	}
	ilDeleteImages(1, &image_id);

	return true;
}

bool ImportTexture::LoadTexture(const char* _path, GameObject* go_fromfbx)
{
	bool ret = true;

	// Name
	std::string name_path = _path;
	uint pos = name_path.find_last_of("\\/");
	name_path = (name_path.substr(pos + 1)).c_str();
	final_path = LIBRARY_TEXTURES_FOLDER + std::string(_path);

	ComponentMaterial *component_material = nullptr;

	if (go_fromfbx != nullptr)
	{
		
		uint id_tex;
		ilGenImages(1, &id_tex);
		ilBindImage(id_tex);
		
		if (ilLoadImage(final_path.c_str()))
		{
			component_material = new ComponentMaterial(go_fromfbx);
			component_material->id_texture = ilutGLBindTexImage();
			component_material->path = final_path;
			component_material->width = ilGetInteger(IL_IMAGE_WIDTH);
			component_material->height = ilGetInteger(IL_IMAGE_HEIGHT);
			go_fromfbx->GetMesh()->id_texture = component_material->id_texture;

			LOG("Added %s to %s", name_path.c_str(), go_fromfbx->GetName().c_str());
		}
		else
		{
			auto error = ilGetError();
			LOG("Error loading texture %s. Error: %s", name_path.c_str(), ilGetString(error));
			ret = false;
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
				LoadTexture(name_path.c_str(), children_game_objects[j]);
			}
		}
		else
		{
			LoadTexture(name_path.c_str(), App->scene->GetSelected());
		}
	}

	return ret;
}

void ImportTexture::DefaultTexture(GameObject* go_texturedefault)
{
	ComponentMaterial *component_material = new ComponentMaterial(go_texturedefault);

	component_material->path = "DefaultTexture";
	component_material->width = 128;
	component_material->height = 128;
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
	glGenTextures(1, &component_material->id_texture);
	glBindTexture(GL_TEXTURE_2D, component_material->id_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	go_texturedefault->GetMesh()->id_default_texture = component_material->id_texture;

}


//bool ImportTexture::Import(const char* import_file, std::string& output_file)
//{
//	uint image_id;
//
//	ilGenImages(1, &image_id); // Grab a new image name.
//	ilBindImage(image_id);
//	ilLoadImage(import_file);
//
//	ILuint size;
//	ILubyte *data;
//	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // To pick a specific DXT compression use 
//	size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer 
//	if (size > 0) {
//		data = new ILubyte[size]; // allocate data buffer   
//		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function        
//		{
//			std::string file;
//			App->fsystem->SplitFilePath(import_file, nullptr, &file, nullptr);
//			output_file = LIBRARY_TEXTURES_FOLDER + file + ".dds";
//			App->fsystem->Save(output_file.c_str(), data, size);
//		}
//		RELEASE_ARRAY(data);
//	}
//	ilDeleteImages(1, &image_id);
//
//	return true;
//}
//
//Texture* TextureImporter::LoadDefault()
//{
//	if (!default_texture)
//	{
//		default_texture = new Texture();
//
//		default_texture->path = "CheckersTexture";
//		default_texture->width = 128;
//		default_texture->height = 128;
//		GLubyte checkImage[128][128][4];
//
//		for (int i = 0; i < 128; i++)
//		{
//			for (int j = 0; j < 128; j++)
//			{
//				int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
//				checkImage[i][j][0] = (GLubyte)c;
//				checkImage[i][j][1] = (GLubyte)c;
//				checkImage[i][j][2] = (GLubyte)c;
//				checkImage[i][j][3] = (GLubyte)255;
//			}
//		}
//
//		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//		glGenTextures(1, &default_texture->id);
//		glBindTexture(GL_TEXTURE_2D, default_texture->id);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
//		glBindTexture(GL_TEXTURE_2D, 0);
//
//		default_texture->mips = 0;
//		default_texture->depth = 0;
//		default_texture->format = "rgba";
//		default_texture->bpp = 0;
//		default_texture->size = sizeof(GLubyte) * 4 * 128 * 128;
//	}
//
//	return default_texture;
//}