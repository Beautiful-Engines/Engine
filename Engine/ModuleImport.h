#ifndef __ModuleImport_H__
#define __ModuleImport_H__

#include "Module.h"

class ComponentMesh;
class GameObject;
class aiNode;
class aiScene;

class ModuleImport : public Module
{
public:
	ModuleImport(bool start_enabled = true);
	~ModuleImport();

	bool Init() override;
	bool Start() override;
	bool CleanUp() override;


	bool LoadFile(const char* path);
	bool LoadFBX(const char* path);
	bool LoadTexture(const char* path, GameObject* go_fromfbx = nullptr);
	void DefaultTexture(GameObject* go_texturedefault);
	void GLBuffer(ComponentMesh *mesh);

	std::string final_path = "";

private:
	void LoadNode(aiNode* node, const aiScene* scene, GameObject* object);
};


#endif // !__ModuleImport_H__