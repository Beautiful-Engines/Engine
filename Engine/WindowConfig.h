#ifndef __WindowConfig_H__
#define __WindowConfig_H__

#include "WindowEngine.h"

class WindowConfig : public WindowEngine
{
public:

	WindowConfig();
	~WindowConfig();

	bool Draw();

private:

	int width = SCREEN_WIDTH * SCREEN_SIZE;
	int height = SCREEN_HEIGHT * SCREEN_SIZE;

	char app_name[20] = TITLE;
	char org_name[20] = ORGANIZATION;
	int fpscap = FPS_CAP;

	float brightness = 1;
	int refresh_rate = 0;
	bool fullscreen = false, fulldesktop = false, borderless = false;
	bool resizable = true;

	ImGuiTextBuffer buff;

	SDL_version compiled_version;
};

#endif // !__WindowConfig_H__