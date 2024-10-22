#ifndef __Application_H__
#define __Application_H__

#define MAX_FPS 120

#include "PCG/pcg_random.hpp"
#include <random>

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
class ModuleImport;
class ModuleFileSystem;
class ModuleResource;
class ModuleTimeManager;



class Application
{
public:
	ModuleWindow * window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleGUI* gui;
	ModuleScene* scene;
	ModuleImport* importer;
	ModuleFileSystem* file_system;
	ModuleResource*  resource;
	ModuleTimeManager* timemanager;

private:

	std::string name = TITLE;
	std::string organization = ORGANIZATION;

	Timer	ms_timer;
	int lastframems = 0;
	int cap_frames = FPS_CAP;
	int cap_ms = 0;
	int fps = 0;
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	float	dt;
	std::list<Module*> list_modules;

	pcg32 uuid;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void SetName(std::string name);
	void SetOrganization(std::string org);
	void SetFPSCap(int capfps);

	std::string GetName() const;
	std::string GetOrganization() const;
	int GetFPSCap();
	std::vector<float> GetFPSVector();
	std::vector<float> GetLastFrameMSVector();
	float GetDt();

	void SetDt(float _dt);

	// Load and Save Configurations
	void LoadDefaultConfig();
	bool LoadConfig();
	void SaveConfig();
	
	uint GenerateNewId();

private:

	void FillFPS();
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;

#endif // !__Application_H__