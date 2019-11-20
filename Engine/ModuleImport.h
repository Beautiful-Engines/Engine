#ifndef __ModuleImport_H__
#define __ModuleImport_H__

#include "Module.h"

class ImportModel;
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


	void ImportFile(const char* _path, bool _change_meta = false);

public:

	ImportModel* import_model;
	ImportMesh* import_mesh;
	ImportTexture* import_texture;

};


#endif // !__ModuleImport_H__