#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Primitive.h"

#include "ModuleImport.h"

#include "ModuleScene.h"

#include "glew\glew.h"


ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	// Initialize ImGUi
	LOG("Creating Scene");

	return true;
}


bool ModuleScene::Start()
{
	CreateGrid();

	GameObject *root = CreateGameObject("root");

	Primitive *sphere = new Primitive(PrimitiveType::SPHERE, 5);
	sphere->SetParent(root);
	game_objects.push_back(sphere);

	//App->importer->LoadMesh("assets/cube.fbx");

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

	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i]->IsEnabled()) 
			game_objects[i]->Update();
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

GameObject* ModuleScene::CreateGameObject(const char* _name)
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

void ModuleScene::AddGameObject(GameObject* game_object)
{
	game_objects.push_back(game_object);
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