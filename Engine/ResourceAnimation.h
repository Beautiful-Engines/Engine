#ifndef _ResourceAnimation_H_
#define _ResourceAnimation_H_

#include "Resource.h"
#include "MathGeoLib\include\Math\MathAll.h"

struct NodeAnimation
{
	std::string name = "";
	uint num_position_keys = 0;
	uint num_rotation_keys = 0;
	uint num_scale_keys = 0;

	float3* position_keys_value = nullptr;
	double* position_keys_times = nullptr;

	Quat* rotation_keys_value = nullptr;
	double* rotation_keys_times = nullptr;

	float3* scale_keys_value = nullptr;
	double* scale_keys_times = nullptr;
	
};

class ResourceAnimation : public Resource
{
public:
	ResourceAnimation(uint UID = 0);
	~ResourceAnimation();

public:
	std::string name = "";
	double duration = 0;
	double ticks_per_second = 0;

	uint num_channels = 0;

	NodeAnimation* nodes = nullptr;
};

#endif // !_ResourceAnimation_H_

