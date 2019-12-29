#ifndef _ModuleResource_H_
#define _ModuleResource_H_

#include "Globals.h"
#include "Module.h"

class Resource;

class ModuleResource : public Module
{
public:
	ModuleResource(bool start_enabled = true);
	~ModuleResource();

	bool Start();

	void LoadFile(const char* _path);
	Resource* Get(uint uid);
	Resource* GetAndUse(uint uid);
	uint GetId(std::string _file);
	std::map<uint, Resource*> GetResources();
	Resource* CreateResource(const char* extension, uint UID = 0);
	void LoadAllAssets();
	void LoadByFolder(const char* _folder);

	bool first_textures = true;

protected:
	std::map<uint, Resource*> resources;
};

#endif // !_Resource_H_
