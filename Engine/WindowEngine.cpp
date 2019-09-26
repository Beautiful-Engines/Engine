#include "WindowEngine.h"


WindowEngine::WindowEngine(){}

WindowEngine::~WindowEngine() {}

void WindowEngine::Show_NotShow()
{
	enabled = !enabled;
}

bool WindowEngine::IsEnabled() const
{
	return enabled;
}

