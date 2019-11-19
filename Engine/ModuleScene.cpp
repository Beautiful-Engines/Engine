#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleGUI.h"
#include "ModuleInput.h"
#include "ModuleImport.h"
#include "ModuleResource.h"
#include "GameObject.h"
#include "Primitive.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "WindowHierarchy.h"
#include "ModuleScene.h"

#include "ResourceModel.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ImportMesh.h"


#include "glew\glew.h"

#include <fstream>
#include <iomanip>

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
	name = "Scene";
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	LOG("Creating Scene");

	return true;
}

bool ModuleScene::Start()
{
	CreateGrid();

	GameObject *root = CreateGameObject("root");
	GameObject *camara = CreateGameObject("camara");
	ComponentCamera* cam = new ComponentCamera(camara);
	camara->AddComponent(cam);

	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float dt)
{

	if (App->renderer3D->grid)
		DrawGrid();

	game_objects[0]->Update();

	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		SaveScene();
	
	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	LOG("Cleaning Scene");
	return true;
}

// Save
void ModuleScene::SaveScene()
{
	LOG("Saving scene into %s", ASSETS_FOLDER"Scene.xfa");
	nlohmann::json json= {
		{"GameObjects", nlohmann::json::array()},
	};
	
	game_objects[0]->Save(json.find("GameObjects"));
	
	std::ofstream ofstream(ASSETS_FOLDER"Scene.xfa");
	ofstream << std::setw(4) << json << std::endl;
}

GameObject* ModuleScene::CreateGameObject(std::string _name)
{
	GameObject *game_object = new GameObject();
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i]->GetParent() == nullptr)
			game_object->SetParent(game_objects[i]);
	}
	game_object->SetName(_name);
	game_objects.push_back(game_object);

	return game_object;
}

void ModuleScene::AddGameObject(GameObject* _game_object)
{
	game_objects.push_back(_game_object);
}

void ModuleScene::DeleteGameObject(GameObject* _game_object)
{

	// Delete children, recursive
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i]->GetParent() == _game_object)
		{
			DeleteGameObject(game_objects[i]);
			i = 0;
		}
	}

	// Delete parent pointer, erase from children
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i] == _game_object->GetParent())
		{
			game_objects[i]->DeleteChild(_game_object);
		}
	}

	// Erase from game_objects
	for (auto iterator_go = game_objects.begin(); iterator_go < game_objects.end(); iterator_go++) {
		if (*iterator_go != nullptr && (*iterator_go) == _game_object)
		{
			RELEASE(*iterator_go);
			iterator_go = game_objects.erase(iterator_go);
			break;
		}
	}
}

GameObject* ModuleScene::GetSelected()
{
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i]->IsFocused())
		{
			return game_objects[i];
		}
	}
	return nullptr;
}

void ModuleScene::ChangeSelected(GameObject* selected)
{
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i] != selected)
		{
			 game_objects[i]->SetFocus(false);
		}
	}
	
}

void ModuleScene::SetSelected(GameObject* go)
{
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i] == go)
		{
			game_objects[i]->SetFocus(true);
		}
	}
}

const std::vector<GameObject*> ModuleScene::GetGameObjects() const
{
	return game_objects;
}

GameObject* ModuleScene::CreateGameObjectModel(ResourceModel* _resource_model)
{
	if (_resource_model != nullptr)
	{
		GameObject* go_model = CreateGameObject(_resource_model->GetName());

		for each (ResourceModel::ModelNode node in _resource_model->nodes)
		{
			GameObject* go_meshes = new GameObject();
			AddGameObject(go_meshes);
			go_meshes->SetName(node.name);
			go_meshes->SetParent(go_model);
			ComponentTransform* transform = go_meshes->GetTransform();
			go_meshes->CreateComponent(ComponentType::MESH);
			ComponentTexture* texture = new ComponentTexture(go_meshes);

			transform->position = node.position;
			transform->rotation = node.rotation;
			transform->scale = node.scale;

			if (node.mesh > 0)
			{
				ResourceMesh* resource_mesh = (ResourceMesh*)App->resource->CreateResource(OUR_MESH_EXTENSION, node.mesh);
				resource_mesh->SetFile(LIBRARY_MESH_FOLDER + std::to_string(node.mesh) + OUR_MESH_EXTENSION);
				resource_mesh->id_texture = node.texture;
				resource_mesh->id_default_texture = App->resource->GetId("DefaultTexture");
				App->importer->import_mesh->LoadMeshFromResource(resource_mesh);
				go_meshes->GetMesh()->AddResourceMesh(resource_mesh);

			}
				
			if (node.texture > 0)
			{
				ResourceTexture* resource_texture = (ResourceTexture*)App->resource->Get(node.texture);
				texture->id_texture = resource_texture->id_texture;
				texture->width = resource_texture->width;
				texture->height = resource_texture->height;
				texture->path = resource_texture->path;
			}
			else
			{
				ResourceTexture* resource_texture = (ResourceTexture*)App->resource->Get(App->resource->GetId("DefaultTexture"));
				texture->id_texture = resource_texture->id_texture;
				texture->width = resource_texture->width;
				texture->height = resource_texture->height;
				texture->path = resource_texture->path;
			}
				
		}
		return go_model;
	}
	return nullptr;
}

void ModuleScene::MouseClicking(const LineSegment& segment)
{
	std::map<float, GameObject*> selected;
	for (uint i = 0; i < game_objects.size(); i++)
	{
		if (segment.Intersects(game_objects[i]->abb))
		{
			float hit_near, hit_far;
			if (segment.Intersects(game_objects[i]->obb, hit_near, hit_far))
				selected[hit_near] = game_objects[i];
		}
	}

	GameObject* Selected = nullptr;
	for (std::map<float, GameObject*>::iterator it = selected.begin(); it != selected.end() && Selected == nullptr; it++)
	{
		const ResourceMesh* mesh = it->second->GetMesh()->GetResourceMesh();
		if (mesh)
		{
				LineSegment local = segment;
				local.Transform(it->second->GetTransform()->transform_matrix.Inverted());
				for (uint v = 0; v < mesh->n_indexes; v += 3)
				{
					uint indexA = mesh->indexes[v];
					float3 v1(mesh->vertices[indexA]);

					uint indexB = mesh->indexes[v + 1];
					float3 v2(mesh->vertices[indexB]);

					uint indexC = mesh->indexes[v + 2];
					float3 v3(mesh->vertices[indexC]);
			
						Triangle triangle(v1, v2, v3);
						LOG("%f,%f,%f", v1, v2, v3);

						if (local.Intersects(triangle, nullptr, nullptr))
						{
							Selected = it->second;
							break;
						}
				}
		}
	}
	for (uint i = 0; i < game_objects.size(); i++)
	{
		if (game_objects[i] == Selected)
		{
			//App->gui->window_hierarchy->select_iterator = i;
			//App->gui->window_hierarchy->node_clicked = i;
			App->scene->SetSelected(game_objects[i]);
			App->scene->ChangeSelected(game_objects[i]);
			LOG("%s", game_objects[i]->GetName().c_str());
		}
	}
}

void ModuleScene::DrawGrid()
{
	glColor3f(1.f, 1.f, 1.f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_grid);
	glVertexPointer(3, GL_INT, 0, NULL);
	glDrawArrays(GL_LINES, 0, grid_vertices);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleScene::CreateGrid()
{
	grid_vertices = (grid_size + 1) * 12;
	
	uint *grides = new uint[grid_vertices];

	for (int i = 0; i <= grid_size * 12; i += 12)
	{
		grides[i] = i / 6 - grid_size; 
		grides[i + 1] = 0; 
		grides[i + 2] = -grid_size;

		grides[i + 3] = i / 6 - grid_size; 
		grides[i + 4] = 0; 
		grides[i + 5] = grid_size;

		grides[i + 6] = grid_size; 
		grides[i + 7] = 0; 
		grides[i + 8] = i / 6 - grid_size;

		grides[i + 9] = -grid_size; 
		grides[i + 10] = 0; 
		grides[i + 11] = i / 6 - grid_size;
	}
	glGenBuffers(1, &id_grid);
	glBindBuffer(GL_ARRAY_BUFFER, id_grid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * grid_vertices, grides, GL_STATIC_DRAW);

	delete[] grides;
}