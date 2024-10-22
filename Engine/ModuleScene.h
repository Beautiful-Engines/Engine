#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Module.h"
#include "Quadtree.h"
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

	void SaveScene(bool _tmp = false);
	bool LoadScene(bool _tmp = false);

	GameObject* CreateGameObject(std::string name);
	void ActiveBBDebug(bool active);
	void AddGameObject(GameObject* game_object, bool _change_name = true);
	void ModuleScene::DeleteGameObject(GameObject* _game_object);
	GameObject* GetSelected();
	GameObject * GetMainCamera();
	void ChangeSelected(GameObject* selected);
	void SetSelected(GameObject * go);
	const std::vector<GameObject*> GetGameObjects() const;

	GameObject* GetGameObject(uint id);
	GameObject* GetGameObjectByName(std::string _name);

	void FrustrumCulling();

	bool AnyCamera();

	void CreateCamera();

	GameObject* ChangeNameByQuantities(GameObject* _game_object);

	void MouseClicking(const LineSegment & segment);

	std::vector<GameObject*> GetStaticGameObjects();

	void CreateQuadtree();

	bool debug_quadtree = true;

private:
	std::vector<GameObject*> game_objects;
	int grid_size = 100;
	uint id_grid = 0;
	uint grid_vertices = 0;
	
	Quadtree quadtree;

private:
	void CreateGrid();
	void DrawGrid();
	void RecalculateQuadtree();

};

#endif // !__ModuleScene_H__