#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleGUI.h"
#include "ModuleInput.h"
#include "ModuleImport.h"
#include "ModuleFileSystem.h"
#include "ModuleResource.h"
#include "GameObject.h"
#include "Primitive.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ModuleTimeManager.h"
#include "WindowHierarchy.h"
#include "ModuleScene.h"

#include "ResourceMesh.h"
#include "ResourceAnimation.h"


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
	CreateCamera();
	CreateQuadtree();
	
	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float dt)
{	

	if (App->timemanager->saved)
	{
		App->scene->SaveScene(true);
		App->timemanager->saved = false;
	}
	if (App->timemanager->load)
	{
		App->scene->LoadScene(true);
		App->timemanager->load = false;
	}
	if (App->renderer3D->grid)
		DrawGrid();

	game_objects[0]->Update(dt);

	if(debug_quadtree)
		quadtree.Draw();

	FrustrumCulling();

	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		SaveScene();
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		LoadScene();

	
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && App->timemanager->play)
	{
		GetGameObjectByName("Assets/skeleton@idle.fbx")->GetAnimation()->blend_id = GetGameObjectByName("Assets/skeleton@idle.fbx")->GetAnimation()->attack_animation->GetId();
		GetGameObjectByName("Assets/skeleton@idle.fbx")->GetAnimation()->blend = true;
		GetGameObjectByName("Assets/skeleton@idle.fbx")->GetAnimation()->blend_loop = false;
	}
	else if (App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT && App->timemanager->play && !GetGameObjectByName("Assets/skeleton@idle.fbx")->GetAnimation()->running)
	{
		GetGameObjectByName("Assets/skeleton@idle.fbx")->GetAnimation()->blend_id = GetGameObjectByName("Assets/skeleton@idle.fbx")->GetAnimation()->run_animation->GetId();
		GetGameObjectByName("Assets/skeleton@idle.fbx")->GetAnimation()->blend = true;
		GetGameObjectByName("Assets/skeleton@idle.fbx")->GetAnimation()->blend_loop = true;
		GetGameObjectByName("Assets/skeleton@idle.fbx")->GetAnimation()->running = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_2) == KEY_UP && App->timemanager->play)
	{
		GetGameObjectByName("Assets/skeleton@idle.fbx")->GetAnimation()->resource_animation = GetGameObjectByName("Assets/skeleton@idle.fbx")->GetAnimation()->idle_animation;
		GetGameObjectByName("Assets/skeleton@idle.fbx")->GetAnimation()->running = false;
	}
	
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
void ModuleScene::SaveScene(bool _tmp)
{
	std::string file;
	if (_tmp)
		file = ASSETS_FOLDER "scene.tmp";
	else
	{
		LOG("Saving scene into %s", ASSETS_FOLDER"Scene.mgr");
		file = ASSETS_FOLDER + std::string("scene") + OUR_SCENE_EXTENSION;
	}
		

	nlohmann::json json = {
	{"GameObjects", nlohmann::json::array()},
	};

	game_objects[0]->Save(json.find("GameObjects"));
	
	std::ofstream ofstream(file);
	ofstream << std::setw(4) << json << std::endl;
	
}

bool ModuleScene::LoadScene(bool _tmp)
{
	std::string file;
	if (_tmp)
		file = ASSETS_FOLDER "scene.tmp";
	else
	{
		LOG("Loading scene from %s", ASSETS_FOLDER"Scene.mgr");
		file = ASSETS_FOLDER + std::string("scene") + OUR_SCENE_EXTENSION;
	}
	
	if (App->file_system->Exists(file.c_str()))
	{
		game_objects.clear();

		std::ifstream ifstream(file);
		nlohmann::json json = nlohmann::json::parse(ifstream);
		nlohmann::json json_game_objects = json.find("GameObjects").value();
		for (nlohmann::json::iterator iterator = json_game_objects.begin(); iterator != json_game_objects.end(); ++iterator)
		{
			nlohmann::json object = iterator.value();
			uint parent_id = object["ParentUID"];

			if (parent_id != 0)
			{
				GameObject* game_object = new GameObject();
				game_object->SetName(object["Name"]);
				game_object->SetId(object["UID"]);
				game_object->is_static = object["IsStatic"];
				if (object["Enable"])
					game_object->Enable();
				else
					game_object->Disable();

				for (uint i = 0; i < game_objects.size(); ++i)
				{
					if (game_objects[i]->GetId() == parent_id)
						game_object->SetParent(game_objects[i]);
				}

				for (nlohmann::json::iterator components_it = object["Components"].begin(); components_it != object["Components"].end(); ++components_it)
				{
					nlohmann::json json_component = components_it.value();
					Component* component;
					if (json_component["type"] == ComponentType::TRANSFORM)
						component = game_object->GetTransform();
					else if (json_component["type"] == ComponentType::MESH)
						component = new ComponentMesh(game_object);
					else if (json_component["type"] == ComponentType::TEXTURE)
						component = new ComponentTexture(game_object);
					else if (json_component["type"] == ComponentType::ANIMATION)
						component = new ComponentAnimation(game_object);
					else if (json_component["type"] == ComponentType::BONE)
						component = new ComponentBone(game_object);
					else if (json_component["type"] == ComponentType::CAMERA)
						component = new ComponentCamera(game_object);

					component->Load(json_component);
				}
				game_object->GetTransform()->GetTransformMatrix();
				AddGameObject(game_object, false);
			}
			else
			{
				GameObject* root = CreateGameObject("root");
				root->SetId(object["UID"]);
			}
		}
	}
	else
	{
		LOG("File %s doesn't exist", ASSETS_FOLDER"Scene.mgr");
	}

	return true;
}


GameObject* ModuleScene::CreateGameObject(std::string _name)
{
	GameObject *game_object = new GameObject();
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i]->GetParent() == nullptr)
			game_object->SetParent(game_objects[0]);
	}
	game_object->SetName(_name);
	AddGameObject(game_object);

	return game_object;
}

void ModuleScene::ActiveBBDebug(bool active)
{
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if(game_objects[i]->GetMesh())
		game_objects[i]->GetMesh()->debug_bb = active;
	}
}

void ModuleScene::AddGameObject(GameObject* _game_object, bool _change_name)
{
	if(_change_name)
		game_objects.push_back(ChangeNameByQuantities(_game_object));
	else
		game_objects.push_back(_game_object);

	CreateQuadtree();
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

GameObject* ModuleScene::GetMainCamera()
{
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i]->GetCamera())
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

GameObject* ModuleScene::GetGameObject(uint id)
{
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i]->GetId() == id)
		{
			return game_objects[i];
		}
	}
	return nullptr;
}

GameObject* ModuleScene::GetGameObjectByName(std::string _name)
{
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		uint pos = game_objects[i]->GetName().find_last_of("_");
		std::string name = game_objects[i]->GetName().substr(0, pos).c_str();
		if (name == _name)
		{
			return game_objects[i];
		}
	}
	return nullptr;
}

void ModuleScene::FrustrumCulling()
{
	std::vector<GameObject*> objects_hit;

	for (uint i = 0; i < App->scene->GetGameObjects().size(); ++i)
	{
		if (AnyCamera())
		{
			if (App->scene->GetGameObjects()[i]->GetCamera() != nullptr)
			{
				if (App->scene->GetGameObjects()[i]->GetCamera()->frustum_culling == true)
				{
					quadtree.Intersect(objects_hit, App->scene->GetGameObjects()[i]->GetCamera()->frustum);
					for (uint j = 0; j < App->scene->GetGameObjects().size(); ++j)
					{
						if (!App->scene->GetGameObjects()[j]->is_static) 
						{
							if (App->scene->GetGameObjects()[j]->GetMesh()) 
							{
								if (App->scene->GetGameObjects()[i]->GetCamera()->frustum.Intersects(App->scene->GetGameObjects()[j]->abb)) {
									objects_hit.push_back(App->scene->GetGameObjects()[j]);
								}
							}
						}
					}
					for (int i = 0; i < objects_hit.size(); ++i) 
					{
						objects_hit[i]->GetMesh()->draw = true;
					}
					/*for (uint j = 0; j < objects_hit.size(); ++j)
					{
						if (objects_hit[j]->GetMesh())
						{
							if (App->scene->GetGameObjects()[i]->GetCamera()->frustum.Intersects(objects_hit[j]->abb))
							{
								if (App->renderer3D->camera->frustum.Intersects(objects_hit[j]->abb))
								{
									objects_hit[j]->GetMesh()->draw = true;
								}
							}
							else
							{
								objects_hit[j]->GetMesh()->draw = false;

							}
						}
					}*/
				}
				else
				{
					quadtree.Intersect(objects_hit, App->renderer3D->camera->frustum);
					for (uint j = 0; j < App->scene->GetGameObjects().size(); ++j)
					{
						if (!App->scene->GetGameObjects()[j]->is_static)
						{
							if (App->scene->GetGameObjects()[j]->GetMesh())
							{
								if (App->renderer3D->camera->frustum.Intersects(App->scene->GetGameObjects()[j]->abb)) {
									objects_hit.push_back(App->scene->GetGameObjects()[j]);
								}
							}
						}
					}
					for (int i = 0; i < objects_hit.size(); ++i)
					{
						if (objects_hit[i]->GetMesh() != nullptr)
							objects_hit[i]->GetMesh()->draw = true;
					}
				}
			}
		}
		else
		{
			CreateCamera();
			LOG("You always need to have at least one camera in the scene!")
		}
	}
}

bool ModuleScene::AnyCamera()
{
	for (uint i = 0; i < App->scene->GetGameObjects().size(); ++i)
	{
		if (App->scene->GetGameObjects()[i]->GetCamera() != nullptr)
		{
			return true;
		}
	}
	return false;
}
void  ModuleScene::CreateCamera()
{
	GameObject *camera = CreateGameObject("camara");
	ComponentCamera* cam = new ComponentCamera(camera);
	cam->frustum_culling = true;
	cam->main_camera = true;
}
GameObject* ModuleScene::ChangeNameByQuantities(GameObject* _game_object)
{
	uint cont = 1;
	std::string name = _game_object->GetName();
	
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i]->GetName() == name + "_" + std::to_string(cont))
		{
			cont++;
			i = 0;
		}
	}

	if (name != "root")
		_game_object->SetName(name.append("_" + std::to_string(cont)));

	return _game_object;
}

void ModuleScene::MouseClicking(const LineSegment& line)
{
	std::map<float, GameObject*> selected;
	quadtree.Intersect(selected, line);

	for (uint i = 0; i < game_objects.size(); i++)
	{
		if (line.Intersects(game_objects[i]->abb))
		{
				float hit_near, hit_far;
				if (line.Intersects(game_objects[i]->obb, hit_near, hit_far))
					selected[hit_near] = game_objects[i];
		}
	}

	GameObject* clicked = nullptr;
	for (std::map<float, GameObject*>::iterator it = selected.begin(); it != selected.end() && clicked == nullptr; it++)
	{
		const ResourceMesh* mesh = it->second->GetMesh()->GetResourceMesh();
		if (mesh)
		{
				LineSegment local = line;
				local.Transform(it->second->GetTransform()->transform_matrix.Inverted());
				for (uint v = 0; v < mesh->n_indexes; v += 3)
				{
					float3 v1(mesh->vertices[mesh->indexes[v]]);
					float3 v2(mesh->vertices[mesh->indexes[v + 1]]);
					float3 v3(mesh->vertices[mesh->indexes[v + 2]]);
			
					Triangle triangle(v1, v2, v3);

					if (local.Intersects(triangle, nullptr, nullptr))
					{
						clicked = it->second;
						break;
					}
				}
		}
	}
	for (uint i = 0; i < game_objects.size(); i++)
	{
		if (game_objects[i] == clicked)
		{
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


std::vector<GameObject*> ModuleScene::GetStaticGameObjects()
{
	std::vector<GameObject*> static_game_objects;
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i]->is_static)
			static_game_objects.push_back(game_objects[i]);
	}

	return static_game_objects;
}


void ModuleScene::CreateQuadtree()
{
	const math::float3 center(0.0f, 0.0f, 0.0f);
	const math::float3 size(100, 40, 100);
	math::AABB boundary;
	boundary.SetFromCenterAndSize(center, size);

	quadtree.Create(boundary);

	RecalculateQuadtree();
}

void ModuleScene::RecalculateQuadtree()
{
	for (uint i = 0; i < GetStaticGameObjects().size(); ++i)
		App->scene->quadtree.Insert(GetStaticGameObjects()[i]);
}