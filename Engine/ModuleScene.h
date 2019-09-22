#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Module.h"

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	void IntersectionTest();

private:

};

#endif // !__ModuleScene_H__