#ifndef __WindowAbout_H__
#define __WindowAbout_H__

#include "WindowEngine.h"

class WindowAbout : public WindowEngine
{
public:

	WindowAbout();
	~WindowAbout();

	bool Draw();
};

#endif // !__WindowAbout_H__