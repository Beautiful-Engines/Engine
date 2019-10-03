#include "Application.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleGUI.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"

#include "nlohmann\json.hpp"
#include <fstream>
#include <iomanip>

Application::Application()
{
	window = new ModuleWindow();
	camera = new ModuleCamera3D();
	gui = new ModuleGUI();
	input = new ModuleInput();
	scene = new ModuleScene();
	renderer3D = new ModuleRenderer3D();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(gui);
	AddModule(input);
	AddModule(scene);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while (item != list_modules.rend())
	{
		delete (*item);
		++item;
	}

	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		++item;
	}

	if (!LoadConfig()) { LoadDefaultConfig(); };

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		++item;
	}

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	lastframems = ms_timer.Read();

	if (!renderer3D->GetVSync() && lastframems < cap_ms && cap_ms > 0)
	{
		if (lastframems < cap_ms)
			SDL_Delay(cap_ms - lastframems);

		lastframems = ms_timer.Read();
	}

	fps = 1000.0 / lastframems;

	dt = 1.0 / fps;

	FillFPS();
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		item++;
	}

	FinishUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while (item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		++item;
	}

	return ret;
}

void Application::SetName(std::string name)
{
	this->name  = name;
	App->window->SetTitle(name.c_str());
}

void Application::SetOrganization(std::string org)
{
	organization = org;
}

void Application::SetFPSCap(int capfps)
{
	cap_frames = capfps;
	if (cap_frames <= 0)
	{
		cap_ms = 1000 / 1;
	}
	else
	{
		cap_ms = 1000 / cap_frames;
	}
}

std::string Application::GetName() const
{
	return name;
}

std::string Application::GetOrganization() const
{
	return organization;
}

int Application::GetFPSCap()
{
	return cap_frames;
}

std::vector<float> Application::GetFPSVector()
{
	return fps_log;
}

std::vector<float> Application::GetLastFrameMSVector()
{
	return ms_log;
}

void Application::FillFPS()
{
	static uint count = 0;
	if (count >= MAX_FPS)
	{
		for (uint i = 0; i < MAX_FPS - 1; ++i)
		{
			fps_log[i] = fps_log[i + 1];
			ms_log[i] = ms_log[i + 1];
		}
		fps_log[count - 1] = fps;
		ms_log[count - 1] = lastframems;

	}
	else
	{
		++count;
		if (fps_log.size() < MAX_FPS)
		{
			for (uint i = 0; i < MAX_FPS; ++i)
			{
				fps_log.push_back(i);
				ms_log.push_back(i);
			}
		}
	}
}

// LOAD AND SAVE
void Application::LoadDefaultConfig()
{
	std::ifstream i("Settings/DefaultConfig.json");
	nlohmann::json load_default_json;
	i >> load_default_json;

	SetName(load_default_json["Application"]["Title"]);
	SetOrganization(load_default_json["Application"]["Organization"]);
	SetFPSCap(load_default_json["Application"]["FPS_cap"]);

	for (std::list<Module*>::iterator iterator = list_modules.begin(); iterator != list_modules.end(); iterator++)
	{
		(*iterator)->LoadDefault(load_default_json);
	}

}
bool Application::LoadConfig()
{
	std::ifstream i("Settings/Config.json");
	
	if (!i.is_open())
	{
		return false;
	}
	else
	{
		nlohmann::json load_json;
		i >> load_json;

		SetName(load_json["Application"]["Title"]);
		SetOrganization(load_json["Application"]["Organization"]);
		SetFPSCap(load_json["Application"]["FPS_cap"]);

		for (std::list<Module*>::iterator iterator = list_modules.begin(); iterator != list_modules.end(); iterator++)
		{
			(*iterator)->Load(load_json);
		}
	}
	
	return true;
}
void Application::SaveConfig()
{
	std::ofstream o("Settings/Config.json");
	nlohmann::json save_json;

	save_json["Application"]["Title"] = name;
	save_json["Application"]["Organization"] = organization;
	save_json["Application"]["FPS_cap"] = cap_frames;

	for (std::list<Module*>::iterator iterator = list_modules.begin(); iterator != list_modules.end(); iterator++)
	{
		(*iterator)->Save(save_json);
	}

	
	o << std::setw(4) << save_json << std::endl;
}



void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}