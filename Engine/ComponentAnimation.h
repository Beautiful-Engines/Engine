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

public:
	ResourceAnimation* resource_animation = nullptr;
	ResourceAnimation* idle_animation = nullptr;
	ResourceAnimation* attack_animation = nullptr;
	ResourceAnimation* run_animation = nullptr;
	ResourceAnimation* blend_animation = nullptr;

	uint blend_id = 0u;
	bool blend = false;
	bool running = false;
	bool interpolation = false;

	float animation_time = 0.0f;
	float blend_animation_time = 0.0f;
	float blend_time = 0.0f;
	float total_blend_time = 0.2f;
	float speed = 1.0f;

	bool loop = true;
	bool blend_loop = false;

};

#endif // !_ComponentAnimation_H_
