#ifndef _ResourceBone_H_
#define _ResourceBone_H_

#include "Resource.h"
#include "MathGeoLib\include\Math\MathAll.h"

struct Weight
{
	uint vertex_id = 0u;
	float weight = 0.0f;
};
class ResourceBone : public Resource
{
public:
	ResourceBone(uint UID = 0);
	~ResourceBone();

public:
	float3 position;
	Quat rotation;
	float3 scale;

	uint num_weights;
	Weight* weights;

	uint id_mesh;

};

#endif // !_ResourceBone_H_