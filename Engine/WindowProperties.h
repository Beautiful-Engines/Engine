#ifndef __WindowProperties_H__
#define __WindowProperties_H__

#include "WindowEngine.h"

class GameObject;
class ComponentTransform;
class ComponentMesh;
class ComponentTexture;
class ComponentCamera;
class ComponentAnimation;
class ResourceTexture;
class ResourceAnimation;

class WindowProperties : public WindowEngine
{
public:
	WindowProperties();
	~WindowProperties();
	bool Draw();

private:
	GameObject *go = nullptr;
	ComponentTransform *trans = nullptr;
	ComponentMesh *mesh = nullptr;
	ComponentTexture *texture = nullptr;
	ComponentCamera *camera = nullptr;
	ComponentAnimation *animation = nullptr;
	ResourceTexture* resource_texture = nullptr;
	ResourceAnimation* resource_animation = nullptr;

};

#endif