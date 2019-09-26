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
	int width, height, refrate = 0;
	float brightness = 0;
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

};

#endif // !__ModuleWindow_H__