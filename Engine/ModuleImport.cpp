#include "Application.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "ModuleScene.h"
#include "ModuleImport.h"
#include "ModuleFileSystem.h"

#include "glmath.h"
#include "glew\glew.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h" 

#include "DeviL/include/il.h"
#include "DeviL/include/ilu.h"
#include "DeviL/include/ilut.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "DeviL/lib/DevIL.lib")
#pragma comment (lib, "DeviL/lib/ILU.lib")
#pragma comment (lib, "DeviL/lib/ILUT.lib")



void LogCallback(const char* text, char* data)
{
	std::string temp_string = text;
	temp_string.erase(std::remove(temp_string.begin(), temp_string.end(), '%'), temp_string.end());
	LOG(temp_string.c_str());
}

ModuleImport::ModuleImport(bool start_enabled) : Module(start_enabled)
{
	name = "Import";
}


ModuleImport::~ModuleImport()
{
}

bool ModuleImport::Init()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = LogCallback;
	aiAttachLogStream(&stream);

	ilInit();
	iluInit();
	ilutInit();
	ilEnable(IL_CONV_PAL);
	ilutEnable(ILUT_OPENGL_CONV);
	ilutRenderer(ILUT_OPENGL);

	return true;
}
bool ModuleImport::Start()
{
	return true;
}

bool ModuleImport::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

bool ModuleImport::LoadFile(const char* _path)
{
	bool ret = true;

	const aiScene * scene = aiImportFile(_path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	//copy file
	std::string normalized_path = _path;
	App->file_system->NormalizePath(normalized_path);
	std::string file;
	App->file_system->SplitFilePath(normalized_path.c_str(), nullptr, &file, nullptr);
	std::string final_path = ASSETS_FOLDER+file;

	App->file_system->CopyFromOutsideFS(normalized_path.c_str(), final_path.c_str());
	
	std::string extension;
	App->file_system->SplitFilePath(final_path.c_str(), nullptr, nullptr, &extension);
	if (extension == "fbx")
	{
		LoadMesh(final_path.c_str());
	}
	else if (extension == "png" || extension == "dds")
	{
		LoadTexture(final_path.c_str());
	}

	return ret;
}

bool ModuleImport::LoadMesh(const char* _path)
{
	bool ret = true;

	// Name
	std::string name_path = _path;
	uint pos = name_path.find_last_of("\\/");
	name_path = (name_path.substr(pos + 1)).c_str();
	pos = name_path.find(".");
	std::string name_object = name_path.substr(0,pos);

	GameObject *go = App->scene->CreateGameObject(name_object);
	
	// Scene
	const aiScene * scene = aiImportFile(_path, aiProcessPreset_TargetRealtime_MaxQuality);

	// Mesh
	if (scene != nullptr && scene->HasMeshes())
	{
		aiMesh *ai_mesh = nullptr;
			
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			name_object = name_object + std::to_string(i);
			GameObject *meshgameobject = new GameObject();
			meshgameobject->SetName(name_object);
			meshgameobject->SetParent(go);
			App->scene->AddGameObject(meshgameobject);
			ComponentMesh *mymesh = new ComponentMesh(meshgameobject);

			ai_mesh = scene->mMeshes[i];

			mymesh->n_vertices = ai_mesh->mNumVertices;
			mymesh->vertices = new float[mymesh->n_vertices * 3];
			memcpy(mymesh->vertices, ai_mesh->mVertices, sizeof(float) * mymesh->n_vertices * 3);

			if (ai_mesh->HasFaces())
			{
				mymesh->n_indexes = ai_mesh->mNumFaces * 3;
				mymesh->indexes = new uint[mymesh->n_indexes];

				for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
				{
					if (ai_mesh->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&mymesh->indexes[j * 3], ai_mesh->mFaces[j].mIndices, sizeof(uint) * 3);
					}
				}
				LOG("New mesh %s, with %i vertices and %i faces has been added", name_path.c_str(), mymesh->n_vertices, mymesh->n_indexes / 3);
			}

			// Normals
			if (ai_mesh->HasNormals())
			{
				mymesh->normals = new aiVector3D[mymesh->n_vertices];
				memcpy(mymesh->normals, ai_mesh->mNormals, sizeof(aiVector3D) * mymesh->n_vertices);

				mymesh->face_center_point = new float[ai_mesh->mNumFaces * 3];
				mymesh->face_normal = new float[ai_mesh->mNumFaces * 3];

				for (uint i = 0; i < mymesh->n_indexes; i += 3)
				{
					uint index = mymesh->indexes[i];
					vec3 vertex0(mymesh->vertices[index * 3], mymesh->vertices[index * 3 + 1], mymesh->vertices[index * 3 + 2]);

					index = mymesh->indexes[i + 1];
					vec3 vertex1(mymesh->vertices[index * 3], mymesh->vertices[index * 3 + 1], mymesh->vertices[index * 3 + 2]);

					index = mymesh->indexes[i + 2];
					vec3 vertex2(mymesh->vertices[index * 3], mymesh->vertices[index * 3 + 1], mymesh->vertices[index * 3 + 2]);

					vec3 v0 = vertex0 - vertex2;
					vec3 v1 = vertex1 - vertex2;
					vec3 n = cross(v0, v1);

					vec3 normalized = normalize(n);

					mymesh->face_center_point[i] = (vertex0.x + vertex1.x + vertex2.x) / 3;
					mymesh->face_center_point[i + 1] = (vertex0.y + vertex1.y + vertex2.y) / 3;
					mymesh->face_center_point[i + 2] = (vertex0.z + vertex1.z + vertex2.z) / 3;

					mymesh->face_normal[i] = normalized.x;
					mymesh->face_normal[i + 1] = normalized.y;
					mymesh->face_normal[i + 2] = normalized.z;
				}
			}

			// UVs
			if (ai_mesh->HasTextureCoords(0))
			{
				mymesh->n_uv = ai_mesh->mNumUVComponents[0];
				mymesh->uv_coords = new float[mymesh->n_vertices * mymesh->n_uv];

				for (uint i = 0; i < mymesh->n_vertices; i++)
				{
					memcpy(&mymesh->uv_coords[i * mymesh->n_uv], &ai_mesh->mTextureCoords[0][i], sizeof(float) * mymesh->n_uv);
				}
			}

			// Materials
			if (ai_mesh->mMaterialIndex >= 0)
			{
				aiString texture_path;
				scene->mMaterials[ai_mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);
				if (texture_path.length > 0)
				{
					LoadTexture(texture_path.C_Str(), meshgameobject);
					DefaultTexture(meshgameobject);
				}
				else
				{
					DefaultTexture(meshgameobject);
				}
			}

			GLBuffer(mymesh);

			ai_mesh = nullptr;
		}

		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", aiGetErrorString());
		ret = false;
	}
	
	return ret;
}

bool ModuleImport::LoadTexture(const char* _path, GameObject* go_fromfbx)
{
	bool ret = true;

	// Name
	std::string name_path = _path;
	uint pos = name_path.find_last_of("\\/");
	name_path = (name_path.substr(pos + 1)).c_str();
	final_path = ASSETS_FOLDER + std::string(_path);

	ComponentMaterial *component_material = nullptr;

	if (go_fromfbx != nullptr)
	{
		component_material = new ComponentMaterial(go_fromfbx);
		uint id_tex;
		ilGenImages(1, &id_tex);
		ilBindImage(id_tex);
		
		if (ilLoadImage(final_path.c_str()))
		{
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
	else
	{
		std::vector<GameObject*> game_objects = App->scene->GetGameObjects();

		/*for (uint i = 0; i < game_objects.size(); ++i)
		{
			if (game_objects[i]->IsFocused())
			{*/
				std::vector<GameObject*> children_game_objects = App->scene->GetSelected()->GetChildren();

				for (uint j = 0; j < children_game_objects.size(); ++j)
				{
					component_material = new ComponentMaterial(children_game_objects[j]);
					uint id_tex;

					ilGenImages(1, &id_tex);
					ilBindImage(id_tex);
					final_path = _path;
					if (ilLoadImage(final_path.c_str()))
					{
						component_material->id_texture = ilutGLBindTexImage();
						component_material->path = final_path;
						component_material->width = ilGetInteger(IL_IMAGE_WIDTH);
						component_material->height = ilGetInteger(IL_IMAGE_HEIGHT);
						children_game_objects[j]->GetMesh()->id_texture = component_material->id_texture;

						LOG("Added %s to %s", name_path.c_str(), App->scene->GetSelected()->GetName().c_str());
					}
					else
					{
						auto error = ilGetError();
						LOG("Error loading texture %s. Error: %s", name_path, ilGetString(error));
						ret = false;
					}

					ilDeleteImages(1, &id_tex);
				}
		/*	}
		}*/
	}

	return ret;
}

void ModuleImport::DefaultTexture(GameObject* go_texturedefault)
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

void ModuleImport::GLBuffer(ComponentMesh *mesh)
{
	glGenBuffers(1, &mesh->id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->n_vertices * 3, mesh->vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &mesh->id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->n_indexes, mesh->indexes, GL_STATIC_DRAW);

	//NORMALS
	if (mesh->normals != nullptr) {
		glGenBuffers(1, &mesh->id_normal);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normal);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->n_vertices * 3, mesh->normals, GL_STATIC_DRAW);
	}

	// UV
	glGenBuffers(1, &mesh->id_uv);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_uv);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh->n_uv * mesh->n_vertices, mesh->uv_coords, GL_STATIC_DRAW);
	
	

}