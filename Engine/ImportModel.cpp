#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleScene.h"
#include "ModuleImport.h"
#include "ModuleResource.h"

#include "GameObject.h"
#include "ImportMesh.h"
#include "ImportTexture.h"
#include "ResourceModel.h"
#include "ResourceMesh.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h" 

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "ImportModel.h"

#include <fstream>
#include <iomanip>

void LogCallback(const char* text, char* data)
{
	std::string temp_string = text;
	temp_string.erase(std::remove(temp_string.begin(), temp_string.end(), '%'), temp_string.end());
	LOG(temp_string.c_str());
}

ImportModel::ImportModel()
{
}

ImportModel::~ImportModel()
{
}

bool ImportModel::Init()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = LogCallback;
	aiAttachLogStream(&stream);

	return true;
}

bool ImportModel::CleanUp()
{
	aiDetachAllLogStreams();

	return false;
}

uint ImportModel::ImportFBX(const char* _path)
{
	uint UID = 0u;
	// Name
	std::string name_path = _path;
	uint pos = name_path.find_last_of("\\/");
	name_path = (name_path.substr(pos + 1)).c_str();
	pos = name_path.find(".");
	std::string name_object = name_path.substr(0, pos);

	ResourceModel* model = (ResourceModel*)App->resource->CreateResource(OUR_MODEL_EXTENSION);

	// Create meta
	nlohmann::json json = {
		{ "exported_file", LIBRARY_MODEL_FOLDER + std::to_string(model->GetId()) + OUR_MODEL_EXTENSION },
		{ "name", _path},
		{ "id", model->GetId() },
		{ "meshes",nlohmann::json::array()}
	};
	// Json Model
	nlohmann::json json_model = {
		{ "name", _path },
		{ "exported_file", LIBRARY_MODEL_FOLDER + std::to_string(model->GetId()) + OUR_MODEL_EXTENSION },
		{ "id", model->GetId() },
		{ "nodes",nlohmann::json::array()}
	};

	// Scene
	const aiScene *scene = aiImportFile(_path, aiProcessPreset_TargetRealtime_MaxQuality);

	// Mesh
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mRootNode->mNumChildren; ++i)
		{
			ResourceMesh* resource_mesh = (ResourceMesh*)App->resource->CreateResource(OUR_MESH_EXTENSION);
			name_object = name_path.substr(0, pos) + std::to_string(i + 1);
			resource_mesh->SetName(name_object);

			// adding meshes to meta
			nlohmann::json::iterator _iterator = json.find("meshes");
			nlohmann::json json_mesh = {
				{"id", resource_mesh->GetId()},
				{ "exported_file", LIBRARY_MESH_FOLDER + std::to_string(resource_mesh->GetId()) + OUR_MESH_EXTENSION }
			};
			_iterator.value().push_back(json_mesh);

			model->nodes.push_back(ImportNode(scene->mRootNode->mChildren[i], scene, resource_mesh));
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", aiGetErrorString());
	}

	// writting to .meta
	std::string meta_path = _path;
	meta_path += ".meta";
	std::ofstream ofstream(meta_path);
	ofstream << std::setw(4) << json << std::endl;

	// writting nodes to model
	std::vector<ResourceModel::ModelNode>::iterator iterator_node = model->nodes.begin();
	uint cont = 0;
	for (; iterator_node != model->nodes.end(); ++iterator_node) {
		nlohmann::json::iterator _iterator = json_model.find("nodes");
		nlohmann::json json_nodes = {
			{"name", iterator_node->name},
			{"id", cont},
			{"parent", model->GetId()},
			{"position", {iterator_node->position.x, iterator_node->position.y, iterator_node->position.z }},
			{"rotation",{iterator_node->rotation.x, iterator_node->rotation.y, iterator_node->rotation.z, iterator_node->rotation.w }},
			{"scale", {iterator_node->scale.x, iterator_node->scale.y, iterator_node->scale.z }},
			{"mesh", iterator_node->mesh},
			{"texture", iterator_node->texture}
		};
		_iterator.value().push_back(json_nodes);
		++cont;
	}
	std::ofstream ofstreammodel(LIBRARY_MODEL_FOLDER + std::to_string(model->GetId()) + OUR_MODEL_EXTENSION);
	ofstreammodel << std::setw(4) << json_model << std::endl;

	return UID;

}

ResourceModel::ModelNode ImportModel::ImportNode(const aiNode* _node, const aiScene* _scene, ResourceMesh* _resource_mesh)
{
	// Transform
	aiVector3D translation, scaling;
	aiQuaternion rotation;
	_node->mTransformation.Decompose(scaling, rotation, translation);

	ResourceModel::ModelNode resource_node;
	resource_node.name = _resource_mesh->GetName();
	resource_node.id = _resource_mesh->GetId();
	resource_node.position = float3(translation.x, translation.y, translation.z);
	resource_node.scale = float3(scaling.x, scaling.y, scaling.z);
	resource_node.rotation = Quat(rotation.x, rotation.y, rotation.z, rotation.w);
	if (resource_node.scale.x > 1000)
		resource_node.scale.x /= 1000;
	if (resource_node.scale.x > 100)
		resource_node.scale.x /= 100;
	if (resource_node.scale.x > 10)
		resource_node.scale.x /= 10;

	// Mesh
	if (_node->mNumMeshes > 0)
	{
		// Mesh
		aiMesh *ai_mesh = nullptr;
		ai_mesh = _scene->mMeshes[_node->mMeshes[0]];
		resource_node.mesh = _resource_mesh->GetId();
		App->importer->import_mesh->Import(_scene, ai_mesh, _resource_mesh);

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
				if (App->file_system->Exists((LIBRARY_TEXTURES_FOLDER + file_name + ".dds").c_str()))
				{
					resource_node.texture = App->importer->import_texture->LoadTexture((LIBRARY_TEXTURES_FOLDER + file_name + ".dds").c_str(), _resource_mesh);
				}
				App->importer->import_texture->DefaultTexture(_resource_mesh);
			}
			else
			{
				App->importer->import_texture->DefaultTexture(_resource_mesh);
			}
		}

		ai_mesh = nullptr;
	}

	if (_node->mNumChildren > 0)
	{
		for (int i = 0; i < _node->mNumChildren; i++)
		{
			ImportNode(_node->mChildren[i], _scene, _resource_mesh);
		}
	}

	return resource_node;
}
