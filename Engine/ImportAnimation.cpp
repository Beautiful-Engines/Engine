#include "Application.h"
#include "ModuleFileSystem.h"
#include "ResourceAnimation.h"
#include "ResourceBone.h"
#include "ResourceGraph.h"

#include "Assimp/include/anim.h"
#include "Assimp/include/mesh.h"

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

// ANIMATION-------------------------------------
void ImportAnimation::Import(aiAnimation* _animation)
{
	ResourceAnimation* anim = new ResourceAnimation();
	if(_animation->mName.length > 0)
		anim->name = _animation->mName.C_Str();
	anim->duration = _animation->mDuration;
	anim->ticks_per_second = _animation->mTicksPerSecond;
	anim->num_channels = _animation->mNumChannels;

	// Nodes
	if (anim->num_channels > 0)
	{
		anim->nodes = new NodeAnimation[anim->num_channels];

		for (uint i = 0; i < anim->num_channels; i++)
		{
			if(_animation->mChannels[i]->mNodeName.length > 0)
				anim->nodes[i].name = _animation->mChannels[i]->mNodeName.C_Str();
			
			//positions
			anim->nodes[i].num_position_keys = _animation->mChannels[i]->mNumPositionKeys;
			anim->nodes[i].position_keys_value = new float3[anim->nodes[i].num_position_keys];
			anim->nodes[i].position_keys_times = new double[anim->nodes[i].num_position_keys];
			for (uint j = 0; j < anim->nodes[i].num_position_keys; j++)
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
			for (uint j = 0; j < anim->nodes[i].num_rotation_keys; j++)
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
			for (uint j = 0; j < anim->nodes[i].num_scale_keys; j++)
			{
				anim->nodes[i].scale_keys_times[j] = _animation->mChannels[i]->mScalingKeys[j].mTime;
				anim->nodes[i].scale_keys_value[j].x = _animation->mChannels[i]->mScalingKeys[j].mValue.x;
				anim->nodes[i].scale_keys_value[j].y = _animation->mChannels[i]->mScalingKeys[j].mValue.y;
				anim->nodes[i].scale_keys_value[j].z = _animation->mChannels[i]->mScalingKeys[j].mValue.z;
			}
		}
	}

	CreateOurAnimation(anim);
	/*RELEASE(anim);*/
}

bool ImportAnimation::CreateOurAnimation(ResourceAnimation* _animation)
{
	// amount of duration / ticks_per_second / channels / name
	uint ranges[2] = { _animation->num_channels, _animation->name.size() };
	uint size = sizeof(ranges);

	// duration
	size += sizeof(double);
	// ticks_per_second
	size += sizeof(double);

	for (uint i = 0; i < _animation->num_channels; i++)
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

	bytes = sizeof(double);
	memcpy(cursor, &_animation->duration, bytes);
	cursor += bytes;

	bytes = sizeof(double);
	memcpy(cursor, &_animation->ticks_per_second, bytes);
	cursor += bytes;

	// nodes
	for (uint i = 0; i < _animation->num_channels; i++)
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
	/*RELEASE_ARRAY(data);*/

	return ret;
}

void ImportAnimation::LoadAnimationFromResource(ResourceAnimation* _animation)
{
	char* buffer;
	uint size = App->file_system->Load(_animation->GetFile(), &buffer);

	char* cursor = buffer;
	// amount of duration / ticks_per_second / channels / name
	uint ranges[4];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	_animation->duration = ranges[0];
	_animation->ticks_per_second = ranges[1];
	_animation->num_channels = ranges[2];
	_animation->name = ranges[3];
	cursor += bytes;

	// loading nodes
	if (_animation->num_channels > 0)
	{
		_animation->nodes = new NodeAnimation[_animation->num_channels];
		for (uint i = 0; i < _animation->num_channels; i++)
		{
			// loading ranges
			uint node_ranges[4];
			uint bytes = sizeof(node_ranges);
			memcpy(node_ranges, cursor, bytes);
			_animation->nodes[i].num_position_keys = node_ranges[0];
			_animation->nodes[i].num_rotation_keys = node_ranges[1];
			_animation->nodes[i].num_scale_keys = node_ranges[2];
			_animation->nodes[i].name = node_ranges[3];
			cursor += bytes;

			// loading position
			bytes = sizeof(double) * _animation->nodes[i].num_position_keys;
			_animation->nodes[i].position_keys_times = new double[_animation->nodes[i].num_position_keys];
			memcpy(_animation->nodes[i].position_keys_times, cursor, bytes);
			cursor += bytes;

			bytes = sizeof(float3) * _animation->nodes[i].num_position_keys;
			_animation->nodes[i].position_keys_value = new float3[_animation->nodes[i].num_position_keys];
			memcpy(_animation->nodes[i].position_keys_value, cursor, bytes);
			cursor += bytes;

			// loading rotation
			bytes = sizeof(double) * _animation->nodes[i].num_rotation_keys;
			_animation->nodes[i].rotation_keys_times = new double[_animation->nodes[i].num_rotation_keys];
			memcpy(_animation->nodes[i].rotation_keys_times, cursor, bytes);
			cursor += bytes;

			bytes = sizeof(Quat) * _animation->nodes[i].num_rotation_keys;
			_animation->nodes[i].rotation_keys_value = new Quat[_animation->nodes[i].num_rotation_keys];
			memcpy(_animation->nodes[i].rotation_keys_value, cursor, bytes);
			cursor += bytes;

			// loading scale
			bytes = sizeof(double) * _animation->nodes[i].num_scale_keys;
			_animation->nodes[i].scale_keys_times = new double[_animation->nodes[i].num_scale_keys];
			memcpy(_animation->nodes[i].scale_keys_times, cursor, bytes);
			cursor += bytes;

			bytes = sizeof(float3) * _animation->nodes[i].num_scale_keys;
			_animation->nodes[i].scale_keys_value = new float3[_animation->nodes[i].num_scale_keys];
			memcpy(_animation->nodes[i].scale_keys_value, cursor, bytes);
			cursor += bytes;
		}
	}

	RELEASE_ARRAY(buffer);
}


// BONE-------------------------------------
void ImportAnimation::ImportBone(aiBone* _bone)
{
	ResourceBone* bone = new ResourceBone();
	bone->num_weights = _bone->mNumWeights;

	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;

	_bone->mOffsetMatrix.Decompose(scale, rotation, position);

	bone->position.x = position.x;
	bone->position.y = position.y;
	bone->position.z = position.z;

	bone->rotation.x = rotation.x;
	bone->rotation.y = rotation.y;
	bone->rotation.z = rotation.z;
	bone->rotation.w = rotation.w;

	bone->scale.x = scale.x;
	bone->scale.y = scale.y;
	bone->scale.z = scale.z;

	if (_bone->mNumWeights > 0)
	{
		bone->weights = new Weight[bone->num_weights];

		for (uint i = 0; i < bone->num_weights; i++)
		{
			bone->weights[i].vertex_id = _bone->mWeights[i].mVertexId;
			bone->weights[i].weight = _bone->mWeights[i].mWeight;
		}

	}

	CreateOurBone(bone);

	RELEASE(bone);
}

bool ImportAnimation::CreateOurBone(ResourceBone* _bone)
{
	// amount of num_weights / id_mesh 
	uint ranges[2] = { _bone->num_weights, _bone->id_mesh };
	uint size = sizeof(ranges) + sizeof(float3) * 2 + sizeof(Quat) + sizeof(uint) * _bone->num_weights + sizeof(float) * _bone->num_weights;

	// Allocate
	char* data = new char[size];
	char* cursor = data;

	// First store ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	bytes = sizeof(float3);
	memcpy(cursor, &_bone->position, bytes);
	cursor += bytes;

	bytes = sizeof(Quat);
	memcpy(cursor, &_bone->rotation, bytes);
	cursor += bytes;

	bytes = sizeof(float3);
	memcpy(cursor, &_bone->scale, bytes);
	cursor += bytes;

	for (uint i = 0; i < _bone->num_weights; i++)
	{
		bytes = sizeof(uint);
		memcpy(cursor, &_bone->weights[i].vertex_id, bytes);
		cursor += bytes;

		bytes = sizeof(float);
		memcpy(cursor, &_bone->weights[i].weight, bytes);
		cursor += bytes;
	}

	std::string file = LIBRARY_BONE_FOLDER + std::to_string(_bone->GetId()) + OUR_BONE_EXTENSION;
	uint ret = App->file_system->Save(file.c_str(), data, size);
	RELEASE_ARRAY(data);

	return ret;
}
void ImportAnimation::LoadBoneFromResource(ResourceBone* _bone)
{
	char* data;
	uint size = App->file_system->Load(_bone->GetFile(), &data);

	char* cursor = data;
	// amount of num_weights / id_mesh 
	uint ranges[2];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	_bone->num_weights = ranges[0];
	_bone->id_mesh = ranges[1];
	cursor += bytes;

	bytes = sizeof(float3);
	memcpy(&_bone->position, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(Quat);
	memcpy(&_bone->rotation, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float3);
	memcpy(&_bone->scale, cursor, bytes);
	cursor += bytes;

	// loading weights
	if (_bone->num_weights > 0)
	{
		_bone->weights = new Weight[_bone->num_weights];
		for (uint i = 0; i < _bone->num_weights; i++)
		{
			bytes = sizeof(uint);
			memcpy(&_bone->weights[i].vertex_id, cursor, bytes);
			cursor += bytes;

			bytes = sizeof(float);
			memcpy(&_bone->weights[i].weight, cursor, bytes);
			cursor += bytes;
		}
	}

	RELEASE_ARRAY(data);
}

// GRAPH---------------------------------------------
bool ImportAnimation::CreateOurGraph(ResourceAnimationGraph* _graph)
{
	return true;
}
void ImportAnimation::LoadGraphFromResource(ResourceAnimationGraph* _graph)
{

}