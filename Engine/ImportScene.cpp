#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleScene.h"
#include "ModuleImport.h"

#include "GameObject.h"
#include "ImportMesh.h"
#include "ImportTexture.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h" 

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "ImportScene.h"

#include <fstream>
#include <iomanip>

void LogCallback(const char* text, char* data)
{
	std::string temp_string = text;
	temp_string.erase(std::remove(temp_string.begin(), temp_string.end(), '%'), temp_string.end());
	LOG(temp_string.c_str());
}

ImportScene::ImportScene()
{
}

ImportScene::~ImportScene()
{
}

bool ImportScene::Init()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = LogCallback;
	aiAttachLogStream(&stream);

	return true;
}

bool ImportScene::CleanUp()
{
	aiDetachAllLogStreams();

	return false;
}

bool ImportScene::ImportFBX(const char* _path)
{
	bool ret = true;

	// Name
	std::string name_path = _path;
	uint pos = name_path.find_last_of("\\/");
	name_path = (name_path.substr(pos + 1)).c_str();
	pos = name_path.find(".");
	std::string name_object = name_path.substr(0, pos);

	GameObject *go = App->scene->CreateGameObject(name_object);

	// Create meta
	nlohmann::json json = {
		{ "original_file", name_path },
		{ "id", go->GetId() },
		{ "meshes",nlohmann::json::array()}
	};

	// Scene
	const aiScene *scene = aiImportFile(_path, aiProcessPreset_TargetRealtime_MaxQuality);

	// Mesh
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mRootNode->mNumChildren; ++i)
		{
			name_object = name_path.substr(0, pos) + std::to_string(i + 1);
			GameObject *meshgameobject = new GameObject();
			meshgameobject->SetName(name_object);
			meshgameobject->SetParent(go);
			// adding meshes to meta
			nlohmann::json::iterator _iterator = json.find("meshes");
			nlohmann::json json_mesh = {
				{"id", meshgameobject->GetId()}
			};
			_iterator.value().push_back(json_mesh);

			ImportNode(scene->mRootNode->mChildren[i], scene, meshgameobject);
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", aiGetErrorString());
		ret = false;
	}

	// writting to .meta
	std::string meta_path = _path;
	meta_path += ".meta";
	std::ofstream ofstream(meta_path);
	ofstream << std::setw(4) << json << std::endl;

	return ret;

}

void ImportScene::ImportNode(const aiNode* _node, const aiScene* _scene, GameObject* _object)
{
	// Transform
	ComponentTransform *mytransform = _object->GetTransform();
	aiVector3D translation, scaling;
	aiQuaternion rotation;
	_node->mTransformation.Decompose(scaling, rotation, translation);
	mytransform->position = float3(translation.x, translation.y, translation.z);
	mytransform->scale = float3(scaling.x, scaling.y, scaling.z);
	if (mytransform->scale.x > 1000)
		mytransform->scale.x /= 1000;
	if (mytransform->scale.x > 100)
		mytransform->scale.x /= 100;
	if (mytransform->scale.x > 10)
		mytransform->scale.x /= 10;
	mytransform->rotation = Quat(rotation.x, rotation.y, rotation.z, rotation.w);

	App->scene->AddGameObject(_object);

	// Mesh
	if (_node->mNumMeshes > 0)
	{
		// Mesh
		aiMesh *ai_mesh = nullptr;
		ai_mesh = _scene->mMeshes[_node->mMeshes[0]];

		App->importer->import_mesh->Import(_scene, ai_mesh, _object);

		// Texture
		if (ai_mesh->mMaterialIndex >= 0)
		{
			aiString texture_path;
			_scene->mMaterials[ai_mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);
			if (texture_path.length > 0)
			{
				std::string file_name;
				App->file_system->SplitFilePath(texture_path.C_Str(), nullptr, &file_name);
				uint pos = file_name.find(".");
				file_name = file_name.substr(0, pos);
				App->importer->import_texture->LoadTexture((file_name + ".dds").c_str(), _object);
				App->importer->import_texture->DefaultTexture(_object);
			}
			else
			{
				App->importer->import_texture->DefaultTexture(_object);
			}
		}

		ai_mesh = nullptr;
	}

	if (_node->mNumChildren > 0)
	{
		for (int i = 0; i < _node->mNumChildren; i++)
		{
			ImportNode(_node->mChildren[i], _scene, _object);
		}
	}

}
