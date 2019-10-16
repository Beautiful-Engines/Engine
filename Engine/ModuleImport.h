#ifndef __ModuleImport_H__
#define __ModuleImport_H__

#include "Module.h"

class ComponentMesh;

class ModuleImport : public Module
{
public:
	ModuleImport(bool start_enabled = true);
	~ModuleImport();

	bool Start() override;
	bool CleanUp() override;


	bool LoadFile(const char* path);
	bool LoadMesh(const char* path);
	bool LoadTexture(const char* path);
	void GLBuffer(ComponentMesh *mesh);

};


#endif // !__ModuleImport_H__