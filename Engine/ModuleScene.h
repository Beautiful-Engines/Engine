#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Module.h"

typedef struct par_shapes_mesh_s par_shapes_mesh;

class ModuleScene : public Module
{
public:
	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	void IntersectionTest();

private:
	uint s_vertex = 0;
	uint s_index = 0;
	par_shapes_mesh* sphere = nullptr;
};

#endif // !__ModuleScene_H__