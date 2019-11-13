#ifndef __ModuleRenderer3D_H__
#define __ModuleRenderer3D_H__

#include "Module.h"
#include "Light.h"
#include "MathGeoLib\include\Math\float3.h"

class ComponentCamera;

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

	void ResizeScene(float w, float h);
	
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
	uint scene_buffer_id = -1;
	uint scene_texture_id;
	uint scene_depth_id;
	
	bool GetVSync();
	void SetVSync(bool VSync);

	void CreateSceneBuffer();

	void DebugDrawCube(const float3 * vertices, Color color) const;

	bool normals = false;
	bool vertex_normals = true;

	bool grid = true;

	ComponentCamera* camera;

};

#endif // !__ModuleRenderer3D_H__