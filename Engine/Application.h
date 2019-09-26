#ifndef __Application_H__
#define __Application_H__


#include <list>
#include <vector>

#include "Globals.h"
#include "Timer.h"

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleGUI;
class ModuleScene;



class Application
{
public:
	ModuleWindow * window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleGUI* gui;
	ModuleScene* scene;

private:

	std::string name;
	std::string organization;

	Timer	ms_timer;
	int lastframems = 0;
	int cap_frames = 0;
	int cap_ms = 0;
	int fps = 0;
	float	dt;
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void SetName(std::string name);
	void SetOrganization(std::string org);
	void SetFPSCap(int capfps);

	std::string GetName();
	std::string GetOrganization();
	int GetFPSCap();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;

#endif // !__Application_H__