#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__

#include "MathGeoLib\include\Math\MathAll.h"
#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* _game_object);
	~ComponentTransform();

	void Save(const nlohmann::json::iterator& _iterator);
	
	float3 position;
	Quat rotation;
	float3 euler;
	float3 scale;

	//local
	float3 local_position;
	Quat local_rotation;
	float3 local_euler;
	float3 local_scale;

	float3 TransformEulerAngles();
	float4x4 GetTransformMatrix();

	float4x4 local_transform_matrix;
	float4x4 transform_matrix;
};
#endif // !__ComponentTransform_H__

