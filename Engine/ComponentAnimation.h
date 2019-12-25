#ifndef _ComponentAnimation_H_
#define _ComponentAnimation_H_

#include "Component.h"

class ResourceAnimation;

class ComponentAnimation :	public Component
{
public:
	ComponentAnimation(GameObject* _game_object);
	~ComponentAnimation();

	void Update(float dt);

	void Save(const nlohmann::json::iterator& _iterator);
	void Load(const nlohmann::json _json);

	void FillBones(uint _id);

public:
	ResourceAnimation* resource_animation = nullptr;

	std::vector<uint> bones_go;
	uint blend_id = 0u;
	bool bones_loaded = false;
	bool blend = false;
	bool smooth = false;
	bool interpolation = false;

	float animation_time = 0.0f;
	float blend_animation_time = 0.0f;
	float blend_time = 0.0f;
	float total_blend_time = 0.2f;
	float speed = 1.0f;

	bool loop = false;
	bool blend_loop = false;
	bool play = false;
	bool pause = false;

};

#endif // !_ComponentAnimation_H_
