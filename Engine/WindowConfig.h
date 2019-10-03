#ifndef __WindowConfig_H__
#define __WindowConfig_H__

#include "WindowEngine.h"

class WindowConfig : public WindowEngine
{
public:

	WindowConfig();
	~WindowConfig();

	bool Draw();

	void LogInput(int key, const char* state, bool mouse = false);

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

	int total_reported_mem = 0;
	int total_actual_mem = 0;
	int peak_reported_mem = 0;
	int peak_actual_mem = 0;
	int accumulated_reported_mem = 0;
	int accumulated_actual_mem = 0;
	int accumulated_alloc_unit = 0;
	int total_alloc_unity_count = 0;
	int peak_alloc_unit_count = 0;

	bool scrolling = false;
	ImGuiTextBuffer buff_input;
	

	SDL_version compiled_version;
};

#endif // !__WindowConfig_H__