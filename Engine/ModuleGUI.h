#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Module.h"

class WindowEngine;
class WindowHierarchy;
class WindowConfig;
class WindowAbout;
class WindowConsole;

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

	void LogInput(int key, const char* state, bool mouse = false);
	void LogDebug(const char* text);

private:
	update_status CreateMainMenuBar();

public:
	WindowHierarchy* window_hierarchy = nullptr;
	WindowConfig* window_config = nullptr;
	WindowAbout* window_about = nullptr;
	WindowConsole* window_console = nullptr;

private:

	std::vector<WindowEngine*> windows_engine;

	bool demo = false;

	bool depth_test = false;
	bool cull_face = false;
	bool lighting = false;
	bool color_material = false;
	bool texture_2D = false;
	bool wireframe_mode = false;

};

#endif // !__ModuleGUI_H__