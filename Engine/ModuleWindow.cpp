#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	name = "Window";
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		if (WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if (WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if (WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

int ModuleWindow::GetWindowHeight()
{
	return height;
}

int ModuleWindow::GetWindowWidth()
{
	return width;
}

float ModuleWindow::GetBrightness()
{
	return brightness;
}

int ModuleWindow::GetRefreshRate()
{
	uint refrate = 0;

	SDL_DisplayMode desktopDisplay;
	if (SDL_GetDesktopDisplayMode(0, &desktopDisplay) == 0)
		refrate = desktopDisplay.refresh_rate;

	return refrate;
}

void ModuleWindow::SetWindowHeight(int height)
{
	this->height = height;
	SDL_SetWindowSize(window, width, height);
}

void ModuleWindow::SetWindowWidth(int width)
{
	this->width = width;
	SDL_SetWindowSize(window, width, height);
}

void ModuleWindow::SetBrightness(float brightness)
{
	SDL_SetWindowBrightness(window, brightness);
}

void ModuleWindow::SetScreenMode(SCREENMODE mode, bool check)
{
	switch (mode)
	{
	case FULLSCREEN:
		if (check) 
		{ 
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN); 
		}
		else SDL_SetWindowFullscreen(window, 0); break;

	case FULLDESKTOP:
		if (check)
		{
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
		else SDL_SetWindowFullscreen(window, 0); break;

	case RESIZABLE:
		if (check)
		{
			SDL_SetWindowResizable(window, SDL_TRUE);
		}
		else SDL_SetWindowResizable(window, SDL_FALSE);	break;

	case BORDERLESS:
		if (check)
		{
			SDL_SetWindowBordered(window, SDL_FALSE);
		}
		else SDL_SetWindowBordered(window, SDL_TRUE); break;

	default:
		break;
	}
}

//Load and Save
bool ModuleWindow::LoadDefault(nlohmann::json &load_default_json)
{
	width = load_default_json[name]["Width"];
	height = load_default_json[name]["Height"];
	brightness = load_default_json[name]["Brightness"];
	fullscreen = load_default_json[name]["Fullscreen"];
	fulldesktop = load_default_json[name]["Fulldesktop"];
	borderless = load_default_json[name]["Borderless"];
	resizable = load_default_json[name]["Resizable"];

	return true;
}

bool ModuleWindow::Load(nlohmann::json &load_json)
{
	width = load_json[name]["Width"];
	height = load_json[name]["Height"];
	brightness = load_json[name]["Brightness"];
	fullscreen = load_json[name]["Fullscreen"];
	fulldesktop = load_json[name]["Fulldesktop"];
	borderless = load_json[name]["Borderless"];
	resizable = load_json[name]["Resizable"];

	return true;
}

bool ModuleWindow::Save(nlohmann::json &save_json)
{
	save_json[name]["Width"] = width;
	save_json[name]["Height"] = height;
	save_json[name]["Brightness"] = brightness;
	save_json[name]["Fullscreen"] = fullscreen;
	save_json[name]["Fulldesktop"] = fulldesktop;
	save_json[name]["Borderless"] = borderless;
	save_json[name]["Resizable"] = resizable;

		

	return true;
}
