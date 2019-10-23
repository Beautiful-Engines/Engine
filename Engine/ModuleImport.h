#ifndef __ModuleImport_H__
#define __ModuleImport_H__

#include "Module.h"

class ComponentMesh;
class GameObject;

class ModuleImport : public Module
{
public:
	ModuleImport(bool start_enabled = true);
	~ModuleImport();

	bool Init() override;
	bool Start() override;
	bool CleanUp() override;


	bool LoadFile(const char* path);
	bool LoadMesh(const char* path);
	bool LoadTexture(const char* path, GameObject* go_fromfbx = nullptr);
	void GLBuffer(ComponentMesh *mesh);

	std::string final_path = "";

};


#endif // !__ModuleImport_H__