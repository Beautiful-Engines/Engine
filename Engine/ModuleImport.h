#ifndef __ModuleImport_H__
#define __ModuleImport_H__

#include "Module.h"

class ImportScene;
class ImportMesh;
class ImportTexture;

class ModuleImport : public Module
{
public:
	ModuleImport(bool start_enabled = true);
	~ModuleImport();

	bool Init() override;
	bool Start() override;
	bool CleanUp() override;


	void LoadFile(const char* path);

public:

	ImportScene* import_scene;
	ImportMesh* import_mesh;
	ImportTexture* import_texture;

};


#endif // !__ModuleImport_H__