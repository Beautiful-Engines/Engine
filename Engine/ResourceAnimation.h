#ifndef _ResourceAnimation_H_
#define _ResourceAnimation_H_

#include "Resource.h"
#include "MathGeoLib\include\Math\MathAll.h"

struct NodeAnimation
{
	std::string name_node = "noname";
	uint num_position_keys = 0;
	uint num_rotation_keys = 0;
	uint num_scale_keys = 0;

	float4x4 lastTransform = float4x4::identity;
	int current_pos_index = -1;
	int current_rot_index = -1;
	int current_scale_index = -1;
	int next_pos_index = -1;
	int next_rot_index = -1;
	int next_scale_index = -1;

	float3* position_keys_value = nullptr;
	double* position_keys_times = nullptr;

	Quat* rotation_keys_value = nullptr;
	double* rotation_keys_times = nullptr;

	float3* scale_keys_value = nullptr;
	double* scale_keys_times = nullptr;
	
	bool CalcCurrentIndex(float time);
	void CalcTransfrom(float time, bool interpolation);
	void AnimBlending(const float4x4& blendtrans, float time);

};

class ResourceAnimation : public Resource
{
public:
	ResourceAnimation(uint UID = 0);
	~ResourceAnimation();

public:
	std::string name_anim = "noname";
	double duration = 0;
	double ticks_per_second = 0;

	uint num_channels = 0;

	NodeAnimation* nodes = nullptr;
};

#endif // !_ResourceAnimation_H_

