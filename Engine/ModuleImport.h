#ifndef __ModuleImport_H__
#define __ModuleImport_H__

#include "Module.h"

class ModuleImport : public Module
{
public:
	ModuleImport(bool start_enabled = true);
	~ModuleImport();

	bool Start() override;
	bool CleanUp() override;


	bool LoadFile(const char* path);
};


#endif // !__ModuleImport_H__