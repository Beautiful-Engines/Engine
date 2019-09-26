#ifndef __WindowEngine_H__
#define __WindowEngine_H__


#include "ImGui\imgui.h"

class WindowEngine
{
public:

	WindowEngine();
	virtual ~WindowEngine();

	virtual bool Draw() = 0;
	virtual bool IsEnabled() const;
	virtual void Show_NotShow();

protected:

	bool enabled = false;
};

#endif // !__WindowEngine_H__