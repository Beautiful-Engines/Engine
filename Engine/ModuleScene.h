#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Module.h"
#include "MathGeoLib\include\MathGeoLib.h"

class GameObject;
class ResourceModel;

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

	void SaveScene();

	GameObject* CreateGameObject(std::string name);
	void AddGameObject(GameObject* game_object);
	void ModuleScene::DeleteGameObject(GameObject* _game_object);
	GameObject* GetSelected();
	GameObject * GetMainCamera();
	void ChangeSelected(GameObject* selected);
	void SetSelected(GameObject * go);
	const std::vector<GameObject*> GetGameObjects() const;

	GameObject* CreateGameObjectModel(ResourceModel* _resource_model);
	void FrustrumCulling();
	GameObject* ChangeNameByQuantities(GameObject* _game_object);

	void MouseClicking(const LineSegment & segment);

private:
	std::vector<GameObject*> game_objects;
	int grid_size = 100;
	uint id_grid = 0;
	uint grid_vertices = 0;
	

private:
	void CreateGrid();
	void DrawGrid();

};

#endif // !__ModuleScene_H__