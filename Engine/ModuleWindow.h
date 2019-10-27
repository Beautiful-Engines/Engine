#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

enum SCREENMODE { FULLSCREEN, FULLDESKTOP, RESIZABLE, BORDERLESS };

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

private:
	int width = SCREEN_WIDTH * SCREEN_SIZE;
	int height = SCREEN_HEIGHT * SCREEN_SIZE;

	float brightness = 1;
	bool fullscreen = false;
	bool fulldesktop = false;
	bool borderless = false;
	bool resizable = true;
public:
	//The window we'll be rendering to
	SDL_Window * window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	int GetWindowHeight();
	int GetWindowWidth();
	float GetBrightness();
	int GetRefreshRate();
	void SetWindowHeight(int height);
	void SetWindowWidth(int width);
	void SetBrightness(float brightness);
	void SetScreenMode(SCREENMODE mode, bool check);

	//Load and Save
	bool LoadDefault(nlohmann::json &load_default_json);
	bool Load(nlohmann::json &load_json);
	bool Save(nlohmann::json &save_json);

};

#endif // !__ModuleWindow_H__