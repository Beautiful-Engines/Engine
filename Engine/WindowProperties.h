#ifndef __WindowProperties_H__
#define __WindowProperties_H__

#include "WindowEngine.h"

class GameObject;
class ComponentTransform;
class ComponentMesh;
class ComponentTexture;
class ComponentCamera;
class ComponentAnimation;
class ComponentBone;
class ResourceTexture;

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
	std::vector < ComponentAnimation*> animations;
	std::vector < ComponentAnimation*> animations_temp;
	std::vector<ComponentBone*> bones;
	std::vector<ComponentBone*> bones_temp;
	ResourceTexture* resource_texture = nullptr;

};

#endif