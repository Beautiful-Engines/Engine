#ifndef __ModuleRenderer3D_H__
#define __ModuleRenderer3D_H__

#include "Module.h"
#include "Light.h"

#define MAX_LIGHTS 8
#define MAX_MESHES 200
#define CHECKERS_HEIGHT 5
#define CHECKERS_WIDTH 5

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	
	//Load and Save
	bool LoadDefault(nlohmann::json &load_default_json);
	bool Load(nlohmann::json &load_json);
	bool Save(nlohmann::json &save_json);
	

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	bool VSync = true;
	
	bool GetVSync();
	void SetVSync(bool VSync);

	bool normals = false;
	bool vertex_normals = true;

	bool grid = true;

};

#endif // !__ModuleRenderer3D_H__