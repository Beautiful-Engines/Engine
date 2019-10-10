#ifndef __ModuleRenderer3D_H__
#define __ModuleRenderer3D_H__

#include "Module.h"
#include "glmath.h"
#include "Light.h"
#include "CustomMesh.h"

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
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	
	//Load and Save
	bool LoadDefault(nlohmann::json &load_default_json);
	bool Load(nlohmann::json &load_json);
	bool Save(nlohmann::json &save_json);

	void GLBuffer(CustomMesh *mesh);
	void Draw(CustomMesh *mesh);
	void DrawNormals(CustomMesh *mesh);


public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	bool VSync = true;
	
	bool GetVSync();
	void SetVSync(bool VSync);

	std::vector<CustomMesh*> meshes;
	bool normals = false;
	bool vertex_normals = false;

	byte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	uint id_texture;
};

#endif // !__ModuleRenderer3D_H__