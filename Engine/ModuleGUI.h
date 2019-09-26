#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Module.h"

class WindowEngine;
class WindowHierarchy;
class WindowConfig;
class WindowAbout;

class ModuleGUI : public Module
{
public:
	ModuleGUI(bool start_enabled = true);
	~ModuleGUI();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	update_status CreateMainMenuBar();

public:
	WindowHierarchy* window_hierarchy = nullptr;
	WindowConfig* window_config = nullptr;
	WindowAbout* window_about = nullptr;

private:

	std::vector<WindowEngine*> windows_engine;

	bool demo = false;

};

#endif // !__ModuleGUI_H__