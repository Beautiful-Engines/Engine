#ifndef __WindowAnimTimer_H__
#define __WindowAnimTimer_H__

#include "WindowEngine.h"

class ComponentAnimation;
class NodeAnimation;

class WindowAnimTimer :	public WindowEngine
{
public:

	WindowAnimTimer();
	~WindowAnimTimer();

	bool FillInfo();
	bool Draw();

public:

	ComponentAnimation* component_animation = nullptr;
	NodeAnimation* node_animation = nullptr;
	float num_frames = 0;

private:

	ImVec2 bar_mov = { 0, 0 };
	ImVec2 mouse_mov = { 0, 0 };

	float button_position = 0.0f;


	float offset = 0.0f;

	bool dragging = false;
	bool scrolled = false;

	float recSize = 1000;
	float zoom = 25;
	float speed = 0.5f;
	float progress = 0.0f;

	float windows_size = 1000.0f;


};

#endif // !__WindowAnimTimer_H__