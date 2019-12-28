#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleScene.h"
#include "ModuleImport.h"
#include "ModuleResource.h"

#include "GameObject.h"
#include "ComponentTexture.h"
#include "ComponentAnimation.h"
#include "ComponentBone.h"
#include "ImportMesh.h"
#include "ImportTexture.h"
#include "ImportAnimation.h"
#include "ResourceModel.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceAnimation.h"
#include "ResourceBone.h"

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
	
	// Scene
	const aiScene *scene = aiImportFile(_path, aiProcessPreset_TargetRealtime_MaxQuality);

	// Create meta
	nlohmann::json json = {
		{ "exported_file", LIBRARY_MODEL_FOLDER + std::to_string(model->GetId()) + OUR_MODEL_EXTENSION },
		{ "name", model->GetName()},
		{ "id", model->GetId() },
		{ "animations", nlohmann::json::array() },
		{ "meshes",nlohmann::json::array()}
	};

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

			model->nodes.push_back(ImportNode(scene->mRootNode->mChildren[i], scene, resource_mesh, model));
		}

		if (scene->HasAnimations())
		{
			for (int i = 0; i < scene->mNumAnimations; ++i)
			{
				model->animations.push_back(App->importer->import_animation->Import(scene->mAnimations[i]));

				// adding animations to meta
				nlohmann::json::iterator _iterator = json.find("animations");
				nlohmann::json json_mesh = {
					{"id_animation", model->animations[i]},
					{ "exported_file_animation", LIBRARY_ANIMATION_FOLDER + std::to_string(model->animations.back()) + OUR_ANIMATION_EXTENSION }
				};
				_iterator.value().push_back(json_mesh);
			}
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

ResourceModel::ModelNode ImportModel::ImportNode(const aiNode* _node, const aiScene* _scene, ResourceMesh* _resource_mesh, ResourceModel* _resource_model, ResourceModel::ModelNode* _resource_node)
{
	// Transform
	aiVector3D translation, scaling;
	aiQuaternion rotation;
	_node->mTransformation.Decompose(scaling, rotation, translation);

	ResourceModel::ModelNode resource_node;
	resource_node.name = _node->mName.C_Str();
	resource_node.id = App->GenerateNewId();
	if(_resource_node == nullptr)
		resource_node.parent = _resource_model->GetId();
	else
		resource_node.parent = _resource_node->id;

	resource_node.position = float3(translation.x, translation.y, translation.z);
	resource_node.scale = float3(scaling.x, scaling.y, scaling.z);
	resource_node.rotation = Quat(rotation.x, rotation.y, rotation.z, rotation.w);
	if (resource_node.scale.x >= 1000)
		resource_node.scale.x /= 1000;
	if (resource_node.scale.x >= 100)
		resource_node.scale.x /= 100;
	if (resource_node.scale.x >= 10)
		resource_node.scale.x /= 10;
	if (resource_node.scale.y >= 1000)
		resource_node.scale.y /= 1000;
	if (resource_node.scale.y >= 100)
		resource_node.scale.y /= 100;
	if (resource_node.scale.y >= 10)
		resource_node.scale.y /= 10;
	if (resource_node.scale.z >= 1000)
		resource_node.scale.z /= 1000;
	if (resource_node.scale.z >= 100)
		resource_node.scale.z /= 100;
	if (resource_node.scale.z >= 10)
		resource_node.scale.z /= 10;

	// Mesh
	if (_node->mNumMeshes > 0)
	{
		// Mesh
		aiMesh *ai_mesh = _scene->mMeshes[_node->mMeshes[0]];
		resource_node.name += "_mesh";
		resource_node.mesh = _resource_mesh->GetId();
		App->importer->import_mesh->Import(_scene, ai_mesh, _resource_mesh);

		// Bones
		if (ai_mesh->HasBones())
		{
			for (int i = 0; i < ai_mesh->mNumBones; i++)
			{
				aiBone* ai_bone = ai_mesh->mBones[i];
				resource_node.bones.push_back(App->importer->import_animation->ImportBone(ai_bone));
			}
		}

		// Texture
		if (ai_mesh->mMaterialIndex >= 0)
		{
			aiString texture_path;
			_scene->mMaterials[ai_mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);
			if (texture_path.length > 0)
			{
				std::string file_name;
				App->file_system->SplitFilePath(texture_path.C_Str(), nullptr, &file_name);

				if (App->resource->Get(App->resource->GetId(ASSETS_FOLDER + file_name)) != nullptr)
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
			ResourceMesh* resource_mesh = (ResourceMesh*)App->resource->CreateResource(OUR_MESH_EXTENSION);
			_resource_model->nodes.push_back(ImportNode(_node->mChildren[i], _scene, resource_mesh, _resource_model, &resource_node));
		}
	}

	return resource_node;
}

void ImportModel::CreateOurModelFile(ResourceModel* _resource)
{
	// Json Model
	nlohmann::json json_model;
	std::vector<ResourceModel::ModelNode>::iterator iterator_node = _resource->nodes.begin();

	for (; iterator_node != _resource->nodes.end(); ++iterator_node) {
		nlohmann::json json_nodes = {
			{"id", iterator_node->id},
			{"name", iterator_node->name},
			{"position", {iterator_node->position.x, iterator_node->position.y, iterator_node->position.z }},
			{"scale", {iterator_node->scale.x, iterator_node->scale.y, iterator_node->scale.z }},
			{"rotation",{iterator_node->rotation.x, iterator_node->rotation.y, iterator_node->rotation.z, iterator_node->rotation.w }},
			{"parent", iterator_node->parent},
			{"texture", iterator_node->texture},
			{"mesh", iterator_node->mesh},
			{"bones", nlohmann::json::array() }
		};

		for (int i = 0; i < iterator_node->bones.size(); ++i)
		{
			nlohmann::json::iterator _iterator = json_nodes.find("bones");
			nlohmann::json json_bones = {
				{"id_bone", iterator_node->bones[i]}
			};
			_iterator.value().push_back(json_bones);
		}

		json_model.push_back(json_nodes);
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
	
	node.id = (*_iterator)["id"];
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

	for (nlohmann::json::iterator iterator = (*_iterator).find("bones").value().begin(); iterator != (*_iterator).find("bones").value().end(); ++iterator)
	{
		node.bones.push_back((*iterator)["id_bone"]);
	}

	_resource->nodes.push_back(node);

	return true;
}

GameObject* ImportModel::CreateModel(ResourceModel* _resource_model)
{
	if (_resource_model != nullptr)
	{
		bool is_last = false;
		std::vector< ResourceModel::ModelNode> vector_nodes;
		GameObject* go_model = App->scene->CreateGameObject(_resource_model->GetName());
		go_model->SetIdNode(_resource_model->GetId() + _resource_model->GetCantities());
		go_model->is_static = true;
		go_model->resource_model = _resource_model;

		// Animation
		std::vector<uint>::iterator iterator_animation = _resource_model->animations.begin();

		for (; iterator_animation != _resource_model->animations.end(); ++iterator_animation) {
			if (*iterator_animation > 0)
			{
				ComponentAnimation* animation = new ComponentAnimation(go_model);
				ResourceAnimation* resource_animation = nullptr;
				if (App->resource->Get(*iterator_animation) != nullptr)
					resource_animation = (ResourceAnimation*)App->resource->GetAndUse(*iterator_animation);
				else
				{
					resource_animation = (ResourceAnimation*)App->resource->CreateResource(OUR_ANIMATION_EXTENSION, *iterator_animation);
					resource_animation->SetFile(LIBRARY_ANIMATION_FOLDER + std::to_string(*iterator_animation) + OUR_ANIMATION_EXTENSION);
					App->importer->import_animation->LoadAnimationFromResource(resource_animation);
				}
				go_model->GetAnimation()->resource_animation = resource_animation;
				go_model->GetAnimation()->idle_animation = resource_animation;
			}
		}

		// Just for delivery, then delete it
		if (go_model->GetName().find("@") < 1000)
		{
			uint pos = go_model->GetName().find("@");
			if (go_model->GetName().find("attack") < 1000)
			{
				App->scene->GetGameObjectByName(go_model->GetName().substr(0, pos) + "@idle.fbx")->GetAnimation()->attack_animation = go_model->GetAnimation()->resource_animation;
				App->scene->DeleteGameObject(go_model);
				return nullptr;
			}
			else if (go_model->GetName().find("run") < 1000)
			{
				App->scene->GetGameObjectByName(go_model->GetName().substr(0, pos) + "@idle.fbx")->GetAnimation()->run_animation = go_model->GetAnimation()->resource_animation;
				App->scene->DeleteGameObject(go_model);
				return nullptr;
			}
		}
		

		for each (ResourceModel::ModelNode node in _resource_model->nodes)
		{
			if (node.id == _resource_model->nodes.back().id)
				is_last = true;

			if (node.bones.size() > 0 && !is_last)
			{
				vector_nodes.push_back(node);
				continue;
			}

			GameObject* go_node = new GameObject();
			go_node->SetName(node.name);
			go_node->SetIdNode(node.id + _resource_model->GetCantities());
			go_node->SetIdNodeParent(node.parent + _resource_model->GetCantities());

			// Parent
			for (uint i = 0; i < App->scene->GetGameObjects().size(); ++i)
			{
				if (App->scene->GetGameObjects()[i]->GetIdNode() == node.parent + _resource_model->GetCantities())
				{
					go_node->SetParent(App->scene->GetGameObjects()[i]);
					break;
				}
			}

			for (uint i = 0; i < App->scene->GetGameObjects().size(); ++i)
			{
				if (App->scene->GetGameObjects()[i]->GetIdNodeParent() == node.id + _resource_model->GetCantities())
				{
					for (uint j = 0; j < App->scene->GetGameObjects().size(); ++j)
					{
						if (App->scene->GetGameObjects()[j]->IsChild(App->scene->GetGameObjects()[i]))
						{
							App->scene->GetGameObjects()[j]->DeleteChild(App->scene->GetGameObjects()[i]);
						}
					}
					App->scene->GetGameObjects()[i]->SetParent(go_node);
				}
			}

			if (go_node->GetParent() == nullptr)
				go_node->SetParent(go_model);

			// Transform
			ComponentTransform* transform = go_node->GetTransform();
			transform->local_position = node.position;
			transform->local_rotation = node.rotation;
			transform->local_scale = node.scale;
			transform->GetTransformMatrix();

			// Mesh
			if (node.mesh > 0)
			{
				ComponentMesh* mesh = new ComponentMesh(go_node);
				ComponentTexture* texture = new ComponentTexture(go_node);
				ResourceMesh* resource_mesh = nullptr;
				if (App->resource->Get(node.mesh) != nullptr)
					resource_mesh = (ResourceMesh*)App->resource->GetAndUse(node.mesh);
				else
				{
					resource_mesh = (ResourceMesh*)App->resource->CreateResource(OUR_MESH_EXTENSION, node.mesh);
					resource_mesh->SetFile(LIBRARY_MESH_FOLDER + std::to_string(node.mesh) + OUR_MESH_EXTENSION);
					App->importer->import_mesh->LoadMeshFromResource(resource_mesh);
				}
				go_node->GetMesh()->AddResourceMesh(resource_mesh);

				// Texture
				if (node.texture > 0)
				{
					texture->texture = (ResourceTexture*)App->resource->GetAndUse(node.texture);
					resource_mesh->id_buffer_texture = texture->texture->id_texture;
				}
				texture->default_texture = (ResourceTexture*)App->resource->GetAndUse(App->resource->GetId("DefaultTexture"));
				resource_mesh->id_buffer_default_texture = texture->default_texture->id_texture;

			}

			App->scene->AddGameObject(go_node);
			/*go_node->is_static = true;*/
			
		}

		if (is_last)
		{
			for each (ResourceModel::ModelNode node in vector_nodes)
			{
				GameObject* go_node = new GameObject();
				go_node->SetName(node.name);
				go_node->SetIdNode(node.id + _resource_model->GetCantities());
				go_node->SetIdNodeParent(node.parent + _resource_model->GetCantities());

				// Parent
				for (uint i = 0; i < App->scene->GetGameObjects().size(); ++i)
				{
					if (App->scene->GetGameObjects()[i]->GetIdNode() == node.parent + _resource_model->GetCantities())
					{
						go_node->SetParent(App->scene->GetGameObjects()[i]);
						break;
					}
				}

				for (uint i = 0; i < App->scene->GetGameObjects().size(); ++i)
				{
					if (App->scene->GetGameObjects()[i]->GetIdNodeParent() == node.id + _resource_model->GetCantities())
					{
						for (uint j = 0; j < App->scene->GetGameObjects().size(); ++j)
						{
							if (App->scene->GetGameObjects()[j]->IsChild(App->scene->GetGameObjects()[i]))
							{
								App->scene->GetGameObjects()[j]->DeleteChild(App->scene->GetGameObjects()[i]);
							}
						}
						App->scene->GetGameObjects()[i]->SetParent(go_node);
					}
				}

				if (go_node->GetParent() == nullptr)
					go_node->SetParent(go_model);

				// Transform
				ComponentTransform* transform = go_node->GetTransform();
				transform->local_position = node.position;
				transform->local_rotation = node.rotation;
				transform->local_scale = node.scale;
				transform->GetTransformMatrix();

				// Mesh
				if (node.mesh > 0)
				{
					ComponentMesh* mesh = new ComponentMesh(go_node);
					ComponentTexture* texture = new ComponentTexture(go_node);
					ResourceMesh* resource_mesh = nullptr;
					if (App->resource->Get(node.mesh) != nullptr)
						resource_mesh = (ResourceMesh*)App->resource->GetAndUse(node.mesh);
					else
					{
						resource_mesh = (ResourceMesh*)App->resource->CreateResource(OUR_MESH_EXTENSION, node.mesh);
						resource_mesh->SetFile(LIBRARY_MESH_FOLDER + std::to_string(node.mesh) + OUR_MESH_EXTENSION);
						App->importer->import_mesh->LoadMeshFromResource(resource_mesh);
					}
					go_node->GetMesh()->AddResourceMesh(resource_mesh);

					// Texture
					if (node.texture > 0)
					{
						texture->texture = (ResourceTexture*)App->resource->GetAndUse(node.texture);
						if(texture->texture != nullptr)
							resource_mesh->id_buffer_texture = texture->texture->id_texture;
					}
					texture->default_texture = (ResourceTexture*)App->resource->GetAndUse(App->resource->GetId("DefaultTexture"));
					resource_mesh->id_buffer_default_texture = texture->default_texture->id_texture;

					// Bone
					std::vector<uint>::iterator iterator_bone = node.bones.begin();

					for (; iterator_bone != node.bones.end(); ++iterator_bone)
					{

						ResourceBone* resource_bone = nullptr;
						if (App->resource->Get(*iterator_bone) != nullptr)
							resource_bone = (ResourceBone*)App->resource->GetAndUse(*iterator_bone);
						else
						{
							resource_bone = (ResourceBone*)App->resource->CreateResource(OUR_BONE_EXTENSION, *iterator_bone);
							resource_bone->SetFile(LIBRARY_BONE_FOLDER + std::to_string(*iterator_bone) + OUR_BONE_EXTENSION);
							App->importer->import_animation->LoadBoneFromResource(resource_bone);
						}

						resource_bone->id_mesh = node.mesh;
						GameObject* go_bone = App->scene->GetGameObjectByName(resource_bone->name_bone);

						ComponentTransform* transform = go_bone->GetTransform();
						transform->local_position = resource_bone->position;
						transform->local_rotation = resource_bone->rotation;
						transform->local_scale = resource_bone->scale;
						transform->GetTransformMatrix();

						ComponentBone* bone = new ComponentBone(go_bone);
						bone->resource_bone = resource_bone;
					}
				}

				App->scene->AddGameObject(go_node);
				/*go_node->is_static = true;*/
			}
		}
		return go_model;
	}
	return nullptr;
}