#ifndef __WindowConsole_H__
#define __WindowConsole_H__

#include "WindowEngine.h"

class WindowConsole : public WindowEngine
{
public:

	WindowConsole();
	~WindowConsole();

	bool Draw();

	void LogDebug(const char* text);

private:
	bool scrolling = false;
	ImGuiTextBuffer buff_debug;
};

#endif // !__WindowConsole_H__
