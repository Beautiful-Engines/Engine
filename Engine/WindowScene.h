#ifndef __WindowScene_H__
#define __WindowScene_H__

#include "WindowEngine.h"
#include "MathGeoLib/include/Math/float2.h"

class WindowScene : public WindowEngine
{
public:
	WindowScene();
	~WindowScene();

	bool Draw();
	void Start();
	bool IsInside(const float2& pos) const;
	void GetSizeWithAspectRatio(int current_width, int current_height);
	int image_w, image_h;

private:
	ImVec2 screen_pos;
	float w, h;
};
#endif
