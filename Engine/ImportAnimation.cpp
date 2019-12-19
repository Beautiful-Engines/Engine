#include "Application.h"
#include "ModuleFileSystem.h"
#include "ResourceAnimation.h"
#include "Assimp/include/anim.h"
#include "ImportAnimation.h"

ImportAnimation::ImportAnimation()
{
}


ImportAnimation::~ImportAnimation()
{
}

bool ImportAnimation::Init()
{
	return true;
}

bool ImportAnimation::CleanUp()
{
	return true;
}

void ImportAnimation::Import(aiAnimation* _animation)
{
	ResourceAnimation* anim = new ResourceAnimation();
	anim->duration = _animation->mDuration;
	anim->ticks_per_second = _animation->mTicksPerSecond;
	anim->num_channels = _animation->mNumChannels;

	// Nodes
	if (anim->num_channels > 0)
	{
		anim->nodes = new NodeAnimation[anim->num_channels];

		for (int i = 0; i < anim->num_channels; i++)
		{
			anim->nodes[i].name = _animation->mChannels[i]->mNodeName.C_Str();
			
			//positions
			anim->nodes[i].num_position_keys = _animation->mChannels[i]->mNumPositionKeys;
			anim->nodes[i].position_keys_value = new float3[anim->nodes[i].num_position_keys];
			anim->nodes[i].position_keys_times = new double[anim->nodes[i].num_position_keys];
			for (int j = 0; j < anim->nodes[i].num_position_keys; j++)
			{
				anim->nodes[i].position_keys_times[j] = _animation->mChannels[i]->mPositionKeys[j].mTime;
				anim->nodes[i].position_keys_value[j].x = _animation->mChannels[i]->mPositionKeys[j].mValue.x;
				anim->nodes[i].position_keys_value[j].y = _animation->mChannels[i]->mPositionKeys[j].mValue.y;
				anim->nodes[i].position_keys_value[j].z = _animation->mChannels[i]->mPositionKeys[j].mValue.z;
			}

			//rotations
			anim->nodes[i].num_rotation_keys = _animation->mChannels[i]->mNumRotationKeys;
			anim->nodes[i].rotation_keys_value = new Quat[anim->nodes[i].num_rotation_keys];
			anim->nodes[i].rotation_keys_times = new double[anim->nodes[i].num_rotation_keys];
			for (int j = 0; j < anim->nodes[i].num_rotation_keys; j++)
			{
				anim->nodes[i].rotation_keys_times[j] = _animation->mChannels[i]->mRotationKeys[j].mTime;
				anim->nodes[i].rotation_keys_value[j].x = _animation->mChannels[i]->mRotationKeys[j].mValue.x;
				anim->nodes[i].rotation_keys_value[j].y = _animation->mChannels[i]->mRotationKeys[j].mValue.y;
				anim->nodes[i].rotation_keys_value[j].z = _animation->mChannels[i]->mRotationKeys[j].mValue.z;
				anim->nodes[i].rotation_keys_value[j].w = _animation->mChannels[i]->mRotationKeys[j].mValue.w;
			}

			//scale
			anim->nodes[i].num_scale_keys = _animation->mChannels[i]->mNumScalingKeys;
			anim->nodes[i].scale_keys_value = new float3[anim->nodes[i].num_scale_keys];
			anim->nodes[i].scale_keys_times = new double[anim->nodes[i].num_scale_keys];
			for (int j = 0; j < anim->nodes[i].num_scale_keys; j++)
			{
				anim->nodes[i].scale_keys_times[j] = _animation->mChannels[i]->mScalingKeys[j].mTime;
				anim->nodes[i].scale_keys_value[j].x = _animation->mChannels[i]->mScalingKeys[j].mValue.x;
				anim->nodes[i].scale_keys_value[j].y = _animation->mChannels[i]->mScalingKeys[j].mValue.y;
				anim->nodes[i].scale_keys_value[j].z = _animation->mChannels[i]->mScalingKeys[j].mValue.z;
			}
		}
	}

	CreateOurAnimation(anim);
	RELEASE(anim);
}

bool ImportAnimation::CreateOurAnimation(ResourceAnimation* _animation)
{
	// amount of duration / ticks_per_second / channels / name
	uint ranges[4] = { _animation->duration, _animation->ticks_per_second, _animation->num_channels, _animation->name.size() };
	uint size = sizeof(ranges);

	for (int i = 0; i < _animation->num_channels; i++)
	{
		uint node_ranges[4] = { _animation->nodes[i].num_position_keys, _animation->nodes[i].num_rotation_keys, _animation->nodes[i].num_scale_keys, _animation->nodes[i].name.size() };
		size += sizeof(node_ranges) + sizeof(float3) * _animation->nodes[i].num_position_keys + sizeof(Quat) * _animation->nodes[i].num_rotation_keys
			+ sizeof(float3) * _animation->nodes[i].num_scale_keys + _animation->nodes[i].name.size();
	}

	// Allocate
	char* data = new char[size];
	char* cursor = data;

	// First store ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	bytes = _animation->name.size();
	memcpy(cursor, _animation->name.c_str(), bytes);
	cursor += bytes;

	// nodes
	for (int i = 0; i < _animation->num_channels; i++)
	{
		uint node_ranges[4] = { _animation->nodes[i].num_position_keys, _animation->nodes[i].num_rotation_keys, _animation->nodes[i].num_scale_keys, _animation->nodes[i].name.size() };

		bytes = sizeof(node_ranges);
		memcpy(cursor, node_ranges, bytes);
		cursor += bytes;

		//position
		bytes = sizeof(double) * _animation->nodes[i].num_position_keys;
		memcpy(cursor, _animation->nodes[i].position_keys_times, bytes);
		cursor += bytes;
		bytes = sizeof(float3) * _animation->nodes[i].num_position_keys;
		memcpy(cursor, _animation->nodes[i].position_keys_value, bytes);
		cursor += bytes;

		//rotation
		bytes = sizeof(double) * _animation->nodes[i].num_rotation_keys;
		memcpy(cursor, _animation->nodes[i].rotation_keys_times, bytes);
		cursor += bytes;
		bytes = sizeof(Quat) * _animation->nodes[i].num_rotation_keys;
		memcpy(cursor, _animation->nodes[i].rotation_keys_value, bytes);
		cursor += bytes;

		//scale
		bytes = sizeof(double) * _animation->nodes[i].num_scale_keys;
		memcpy(cursor, _animation->nodes[i].scale_keys_times, bytes);
		cursor += bytes;
		bytes = sizeof(float3) * _animation->nodes[i].num_scale_keys;
		memcpy(cursor, _animation->nodes[i].scale_keys_value, bytes);
		cursor += bytes;

		bytes = _animation->nodes[i].name.size();
		memcpy(cursor, _animation->nodes[i].name.c_str(), bytes);
		cursor += bytes;
	}

	std::string file = LIBRARY_ANIMATION_FOLDER + std::to_string(_animation->GetId()) + OUR_ANIMATION_EXTENSION;
	uint ret = App->file_system->Save(file.c_str(), data, size);
	RELEASE_ARRAY(data);

	return ret;
}

void ImportAnimation::LoadAnimationFromResource(ResourceAnimation* _resource_animation)
{
	char* buffer;
	uint size = App->file_system->Load(_resource_animation->GetFile(), &buffer);

	char* cursor = buffer;
	// amount of duration / ticks_per_second / channels / name
	uint ranges[4];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	_resource_animation->duration = ranges[0];
	_resource_animation->ticks_per_second = ranges[1];
	_resource_animation->num_channels = ranges[2];
	_resource_animation->name = ranges[3];
	cursor += bytes;

	// loading nodes
	if (_resource_animation->num_channels > 0)
	{
		_resource_animation->nodes = new NodeAnimation[_resource_animation->num_channels];
		for (int i = 0; i < _resource_animation->num_channels; i++)
		{
			// loading ranges
			uint node_ranges[4];
			uint bytes = sizeof(node_ranges);
			memcpy(node_ranges, cursor, bytes);
			_resource_animation->nodes[i].num_position_keys = node_ranges[0];
			_resource_animation->nodes[i].num_rotation_keys = node_ranges[1];
			_resource_animation->nodes[i].num_scale_keys = node_ranges[2];
			_resource_animation->nodes[i].name = node_ranges[3];
			cursor += bytes;

			// loading position
			bytes = sizeof(double) * _resource_animation->nodes[i].num_position_keys;
			_resource_animation->nodes[i].position_keys_times = new double[_resource_animation->nodes[i].num_position_keys];
			memcpy(_resource_animation->nodes[i].position_keys_times, cursor, bytes);
			cursor += bytes;

			bytes = sizeof(float3) * _resource_animation->nodes[i].num_position_keys;
			_resource_animation->nodes[i].position_keys_value = new float3[_resource_animation->nodes[i].num_position_keys];
			memcpy(_resource_animation->nodes[i].position_keys_value, cursor, bytes);
			cursor += bytes;

			// loading rotation
			bytes = sizeof(double) * _resource_animation->nodes[i].num_rotation_keys;
			_resource_animation->nodes[i].rotation_keys_times = new double[_resource_animation->nodes[i].num_rotation_keys];
			memcpy(_resource_animation->nodes[i].rotation_keys_times, cursor, bytes);
			cursor += bytes;

			bytes = sizeof(Quat) * _resource_animation->nodes[i].num_rotation_keys;
			_resource_animation->nodes[i].rotation_keys_value = new Quat[_resource_animation->nodes[i].num_rotation_keys];
			memcpy(_resource_animation->nodes[i].rotation_keys_value, cursor, bytes);
			cursor += bytes;

			// loading scale
			bytes = sizeof(double) * _resource_animation->nodes[i].num_scale_keys;
			_resource_animation->nodes[i].scale_keys_times = new double[_resource_animation->nodes[i].num_scale_keys];
			memcpy(_resource_animation->nodes[i].scale_keys_times, cursor, bytes);
			cursor += bytes;

			bytes = sizeof(float3) * _resource_animation->nodes[i].num_scale_keys;
			_resource_animation->nodes[i].scale_keys_value = new float3[_resource_animation->nodes[i].num_scale_keys];
			memcpy(_resource_animation->nodes[i].scale_keys_value, cursor, bytes);
			cursor += bytes;
		}
	}

	RELEASE_ARRAY(buffer);
}
