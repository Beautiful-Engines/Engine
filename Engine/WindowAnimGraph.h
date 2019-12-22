#ifndef __WindowAnimGraph_H__
#define __WindowAnimGraph_H__

#include "WindowEngine.h"

struct Node;

class WindowAnimGraph : public WindowEngine
{
public:
	WindowAnimGraph();
	~WindowAnimGraph();

	bool Draw();

	void DrawAnimationBox(Node* node);

	void SelectNode(Node* node);
};
#endif

