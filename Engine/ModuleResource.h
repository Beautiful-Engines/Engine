#ifndef _ModuleResource_H_
#define _ModuleResource_H_

#include "Globals.h"
class Resource;

class ModuleResource : public Module
{
public:
	ModuleResource(bool start_enabled = true);
	~ModuleResource();

	bool Start();

	void LoadFile(const char* _path);
	Resource* Get(uint uid);
	uint GetId(std::string _file);
	Resource* CreateResource(const char* extension, uint UID = 0);
	void LoadAllAssets();
	
private:
	

protected:
	std::map<uint, Resource*> resources;
};

#endif // !_Resource_H_