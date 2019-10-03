#ifndef __WindowEngine_H__
#define __WindowEngine_H__


#include "ImGui\imgui.h"
#include "glew\glew.h"

class WindowEngine
{
public:

	WindowEngine();
	virtual ~WindowEngine();

	virtual bool Draw() = 0;

	bool enabled = false;

};

#endif // !__WindowEngine_H__