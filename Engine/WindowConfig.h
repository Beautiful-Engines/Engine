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

	std::string app_name = "";
	std::string org_name = "";
	int fpscap = 0;
	std::vector<float> fps_log;
	std::vector<float> ms_log;

	float brightness = 1;
	int refresh_rate = 0;
	bool fullscreen = false, fulldesktop = false, borderless = false;
	bool resizable = true;
};

#endif // !__WindowConfig_H__