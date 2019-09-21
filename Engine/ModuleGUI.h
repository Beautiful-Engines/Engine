#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Module.h"

class ModuleGUI : public Module
{
public:
	ModuleGUI(Application* app, bool start_enabled = true);
	~ModuleGUI();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	update_status CreateMainMenuBar();
	void ShowWindow1(bool *window);
	void IntersectionTest();
	

private:
	bool window1 = false;
	bool demo = false;
};

#endif // !__ModuleGUI_H__