#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Module.h"

class GameObject;

class ModuleScene : public Module
{
public:
	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	GameObject* CreateGameObject(const char* name);
	void AddGameObject(GameObject* game_object);
	const std::vector<GameObject*> GetGameObjects() const;

private:
	std::vector<GameObject*> game_objects;
	uint grid_size = 100;
	uint id_grid = 0;
	uint grid_vertices = 0;
	

private:
	void CreateGrid();
	void DrawGrid();
	

};

#endif // !__ModuleScene_H__