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
	model->SetName(_path);
	
	// Create meta
	nlohmann::json json = {
		{ "exported_file", LIBRARY_MODEL_FOLDER + std::to_string(model->GetId()) + OUR_MODEL_EXTENSION },
		{ "name", model->GetName()},
		{ "id", model->GetId() },
		{ "meshes",nlohmann::json::array()}
	};

	// Scene
	const aiScene *scene = aiImportFile(_path, aiProcessPreset_TargetRealtime_MaxQuality);

	// Mesh
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mRootNode->mNumChildren; ++i)
		{
			ResourceMesh* resource_mesh = (ResourceMesh*)App->resource->CreateResource(OUR_MESH_EXTENSION);
			resource_mesh->SetName(name_object);
			resource_mesh->SetFile(LIBRARY_MESH_FOLDER + std::to_string(resource_mesh->GetId()) + OUR_MESH_EXTENSION);

			// adding meshes to meta
			nlohmann::json::iterator _iterator = json.find("meshes");
			nlohmann::json json_mesh = {
				{"id", resource_mesh->GetId()},
				{ "exported_file", resource_mesh->GetFile() }
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

	// create our model
	CreateOurModelFile(model);

	

	return UID;

}

ResourceModel::ModelNode ImportModel::ImportNode(const aiNode* _node, const aiScene* _scene, ResourceMesh* _resource_mesh)
{
	// Transform
	aiVector3D translation, scaling;
	aiQuaternion rotation;
	_node->mTransformation.Decompose(scaling, rotation, translation);

	ResourceModel::ModelNode resource_node;
	resource_node.name = _node->mName.C_Str();
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
		resource_node.name += "_mesh";
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

				if (App->resource->Get(App->resource->GetId(ASSETS_FOLDER + file_name)))
					resource_node.texture = App->resource->Get(App->resource->GetId(ASSETS_FOLDER + file_name))->GetId();
				else
				{
					uint pos = file_name.find(".");
					file_name = file_name.substr(0, pos);
					if (App->file_system->Exists((LIBRARY_TEXTURES_FOLDER + file_name + ".dds").c_str()))
						resource_node.texture = App->importer->import_texture->LoadTexture((LIBRARY_TEXTURES_FOLDER + file_name + ".dds").c_str(), _resource_mesh);
				}
				
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

void ImportModel::CreateOurModelFile(ResourceModel* _resource)
{
	// Json Model
	nlohmann::json json_model;
	std::vector<ResourceModel::ModelNode>::iterator iterator_node = _resource->nodes.begin();
	uint cont = 0;
	for (; iterator_node != _resource->nodes.end(); ++iterator_node) {
		nlohmann::json json_nodes = {
			{"name", iterator_node->name},
			{"id", cont},
			{"parent", _resource->GetId()},
			{"position", {iterator_node->position.x, iterator_node->position.y, iterator_node->position.z }},
			{"rotation",{iterator_node->rotation.x, iterator_node->rotation.y, iterator_node->rotation.z, iterator_node->rotation.w }},
			{"scale", {iterator_node->scale.x, iterator_node->scale.y, iterator_node->scale.z }},
			{"mesh", iterator_node->mesh},
			{"texture", iterator_node->texture}
		};
		json_model.push_back(json_nodes);
		++cont;
	}
	std::ofstream ofstreammodel(LIBRARY_MODEL_FOLDER + std::to_string(_resource->GetId()) + OUR_MODEL_EXTENSION);
	ofstreammodel << std::setw(4) << json_model << std::endl;
}

bool ImportModel::LoadModel(ResourceModel* _resource)
{
	std::string path = _resource->GetFile();

	std::ifstream ifstream(path);
	nlohmann::json json = nlohmann::json::parse(ifstream);

	for (nlohmann::json::iterator iterator = json.begin(); iterator != json.end(); ++iterator)
	{
		LoadNode(iterator, _resource);
	}

	return true;
}

bool ImportModel::LoadNode(nlohmann::json::iterator _iterator, ResourceModel* _resource)
{
	ResourceModel::ModelNode node;

	node.name = (*_iterator)["name"].get<std::string>();

	node.position.x = (*_iterator)["position"][0];
	node.position.y = (*_iterator)["position"][1];
	node.position.z = (*_iterator)["position"][2];

	node.scale.x = (*_iterator)["scale"][0];
	node.scale.y = (*_iterator)["scale"][1];
	node.scale.z = (*_iterator)["scale"][2];

	node.rotation.x = (*_iterator)["rotation"][0];
	node.rotation.y = (*_iterator)["rotation"][1];
	node.rotation.z = (*_iterator)["rotation"][2];
	node.rotation.w = (*_iterator)["rotation"][3];

	node.parent = (*_iterator)["parent"];
	node.texture = (*_iterator)["texture"];
	node.mesh = (*_iterator)["mesh"];

	_resource->nodes.push_back(node);

	return true;
}