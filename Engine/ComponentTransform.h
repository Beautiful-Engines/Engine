#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__

#include "MathGeoLib\include\Math\MathAll.h"
#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* _game_object);
	~ComponentTransform();
	
	float3 position;
	Quat rotation;
	float3 scale;

	//local
	float3 local_position;
	Quat local_rotation;
	float3 local_scale;
};
#endif

