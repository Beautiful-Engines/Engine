#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Primitive.h"
#include "ModuleScene.h"

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
	
	Primitive *root = new Primitive(PrimitiveType::OTHER, 0);
	root->SetName("root");
	game_objects.push_back(root);

	Primitive *sphere = new Primitive(PrimitiveType::SPHERE, 5);
	game_objects.push_back(root);

	return true;
}


bool ModuleScene::Start()
{
	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float dt)
{
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

GameObject* ModuleScene::CreateGameObject()
{
	return new GameObject();
}

void ModuleScene::AddGameObject(GameObject* game_object)
{
	game_objects.push_back(game_object);
}
