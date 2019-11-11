#ifndef __WindowHierarchy_H__
#define __WindowHierarchy_H__

#include "WindowEngine.h"
class GameObject;

class WindowHierarchy : public WindowEngine
{
public:

	WindowHierarchy();
	~WindowHierarchy();

	bool Draw();

	void DrawNode(GameObject * go);

	int select_iterator = -1;
private:
	void DragAndDrop(GameObject* go);
};

#endif // !__WindowHierarchy_H__
