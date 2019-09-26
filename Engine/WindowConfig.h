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

	float brightness = 0;
	int refresh_rate = 0;
	bool fullscreen, fulldesktop, resizable, borderless;
};

#endif // !__WindowConfig_H__