#ifndef __WindowScene_H__
#define __WindowScene_H__

#include "WindowEngine.h"
#include "glmath.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "MathGeoLib\include\Math\float3.h"
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
	bool IsInside(const float2& pos) const;
	void GetSizeWithAspectRatio(int current_width, int current_height);
	void TransformInputs();
	void DrawGuizmo();
	float w, h;

private:
	LineSegment Ray;
	ImVec2 screen_pos;
};
#endif // !__WindowScene_H__

