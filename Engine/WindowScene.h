#ifndef __WindowScene_H__
#define __WindowScene_H__

#include "WindowEngine.h"
#include "MathGeoLib/include/Math/float2.h"
#include "ImGuizmo.h"

class WindowScene : public WindowEngine
{
public:
	WindowScene();
	~WindowScene();

	ImGuizmo::OPERATION guizmo_operation;
	ImGuizmo::MODE guizmo_mode;

	bool Draw();
	void Start();
	bool IsInside(const float2& pos) const;
	void GetSizeWithAspectRatio(int current_width, int current_height);
	void TransformInputs();
	void DrawGuizmo();
	float w, h;

private:
	ImVec2 screen_pos;
};
#endif
