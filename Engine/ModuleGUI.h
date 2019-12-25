#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Module.h"

class WindowEngine;
class WindowHierarchy;
class WindowConfig;
class WindowAbout;
class WindowConsole;
class WindowProperties;
class WindowPrimitives;
class WindowProject;
class WindowResources;
class WindowAnimTimer;
class WindowScene;

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
	WindowPrimitives* window_primitives = nullptr;
	WindowProperties* window_properties = nullptr;
	WindowProject* window_project = nullptr;
	WindowResources* window_resources = nullptr;
	WindowAnimTimer* window_anim_timer = nullptr;
	WindowScene* window_scene = nullptr;

private:

	std::vector<WindowEngine*> windows_engine;

	bool demo = false;

	bool depth_test = true;
	bool cull_face = true;
	bool lighting = true;
	bool color_material = true;
	bool texture_2D = true;
	bool normals = false;
	bool vertex_normals = false;
	bool play = false;
	bool debug_bb = true;


};

#endif // !__ModuleGUI_H__