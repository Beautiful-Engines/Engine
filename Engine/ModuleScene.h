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

	GameObject* CreateGameObject();
	void AddGameObject(GameObject* game_object);

private:
	std::vector<GameObject*> game_objects;
};

#endif // !__ModuleScene_H__