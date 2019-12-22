#include "Application.h"
#include "ResourceAnimation.h"
#include "ModuleTimeManager.h" 

ResourceAnimation::ResourceAnimation(uint UID) : Resource(Resource::RESOURCE_TYPE::RESOURCE_ANIMATION, UID)
{

}

ResourceAnimation::~ResourceAnimation()
{
}

bool NodeAnimation::CalcCurrentIndex(float time, bool game_mode)
{
	bool ret = false;

	if ((App->timemanager->state == EDITOR && !game_mode) || current_pos_index == -1 || current_rot_index == -1 || current_scale_index == -1 || next_pos_index == -1 || next_rot_index == -1 || next_scale_index == -1)
	{
		current_pos_index = current_rot_index = current_scale_index = 0;
		next_pos_index = next_rot_index = next_scale_index = 1;
		return true;
	}

	for (int i = 0; i < num_position_keys; i++)
	{
		if (position_keys_times[i] <= time && (i + 1 >= num_position_keys || position_keys_times[i + 1] > time))
		{
			current_pos_index = i;

			next_pos_index = current_pos_index + 1;

			if (next_pos_index >= num_position_keys && num_position_keys > 1)
			{
				next_pos_index = 0;
			}


			ret = true;
			break;
		}
	}
	for (int i = 0; i < num_rotation_keys; i++)
	{
		if (rotation_keys_times[i] <= time && (i + 1 >= num_rotation_keys || rotation_keys_times[i + 1] > time))
		{
			current_rot_index = i;

			next_rot_index = current_rot_index + 1;

			if (next_rot_index >= num_rotation_keys && num_rotation_keys > 1)
			{
				next_rot_index = 0;
			}

			ret = true;
			break;
		}
	}
	for (int i = 0; i < num_scale_keys; i++)
	{
		if (scale_keys_times[i] <= time && (i + 1 >= num_scale_keys || scale_keys_times[i + 1] > time))
		{
			current_scale_index = i;

			next_scale_index = current_scale_index + 1;

			if (next_scale_index >= num_scale_keys && num_scale_keys > 1)
			{
				next_scale_index = 0;
			}

			ret = true;
			break;
		}
	}
	return ret;
}

void NodeAnimation::CalcTransfrom(float time, bool interpolation)
{
	float time_pos = 0.0f, time_scale = 0.0f, time_rot = 0.0f;

	float3 position_1 = { position_keys_value[current_pos_index].x, position_keys_value[current_pos_index].y, position_keys_value[current_pos_index].z };
	Quat rotation_1 = { rotation_keys_value[current_rot_index].x, rotation_keys_value[current_rot_index].y, rotation_keys_value[current_rot_index].z, rotation_keys_value[current_rot_index].w };
	float3 scale_1 = { scale_keys_value[current_scale_index].x, scale_keys_value[current_scale_index].y, scale_keys_value[current_scale_index].z };

	float3 position_2 = { position_keys_value[next_pos_index].x, position_keys_value[next_pos_index].y, position_keys_value[next_pos_index].z };
	Quat rotation_2 = { rotation_keys_value[next_rot_index].x, rotation_keys_value[next_rot_index].y, rotation_keys_value[next_rot_index].z, rotation_keys_value[next_rot_index].w };
	float3 scale_2 = { scale_keys_value[next_scale_index].x, scale_keys_value[next_scale_index].y, scale_keys_value[next_scale_index].z };


	time_pos = ((time - position_keys_times[current_pos_index]) / (position_keys_times[next_pos_index] - position_keys_times[current_pos_index]));
	time_rot = ((time - rotation_keys_times[current_rot_index]) / (rotation_keys_times[next_rot_index] - rotation_keys_times[current_rot_index]));
	time_scale = ((time - scale_keys_times[current_scale_index]) / (scale_keys_times[next_scale_index] - scale_keys_times[current_scale_index]));

	if (time_pos < 0) time_pos = 0;
	if(time_rot < 0) time_rot = 0;
	if(time_scale < 0) time_scale = 0;

	float3 position = position_1.Lerp(position_2, time_pos);
	Quat rotation = rotation_1.Slerp(rotation_2, time_rot);
	float3 scale = scale_1.Lerp(scale_2, time_scale);

	if (interpolation)
	{
		lastTransform.Set(float4x4::FromTRS(position, rotation, scale));
	}
	else
	{
		lastTransform.Set(float4x4::FromTRS(position_1, rotation_1, scale_1));
	}

}

void NodeAnimation::AnimBlending(const float4x4& blendtrans, float time)
{
	float3 position_1;
	Quat rotation_1;
	float3 scale_1;

	lastTransform.Decompose(position_1, rotation_1, scale_1);

	float3 position_2;
	Quat rotation_2;
	float3 scale_2;

	blendtrans.Decompose(position_2, rotation_2, scale_2);

	float3 finalpos = position_1.Lerp(position_2, time);
	Quat finalrot = rotation_1.Slerp(rotation_2, time);
	float3 finalscale = scale_1.Lerp(scale_2, time);

	lastTransform.Set(float4x4::FromTRS(finalpos, finalrot, finalscale));
}
