#include "Application.h"
#include "ModuleResource.h"
#include "ModuleScene.h"
#include "ModuleTimeManager.h"
#include "GameObject.h"
#include "ResourceAnimation.h"
#include "ComponentAnimation.h"


ComponentAnimation::ComponentAnimation(GameObject* _game_object) : Component(_game_object, ComponentType::ANIMATION)
{
}

ComponentAnimation::~ComponentAnimation()
{
}

void ComponentAnimation::Update(float dt)
{

	if (resource_animation != nullptr)
	{
		
		animation_time += dt * speed;

		if (animation_time > resource_animation->duration / resource_animation->ticks_per_second && loop)
		{
			animation_time -= resource_animation->duration / resource_animation->ticks_per_second;
		}
		else if (animation_time > resource_animation->duration / resource_animation->ticks_per_second)
		{
			if(running)
				resource_animation = run_animation;
			else
				resource_animation = idle_animation;
			loop = true;
		}

		for (int i = 0; i < resource_animation->num_channels; i++)
		{
			GameObject* go = App->scene->GetGameObjectByName(resource_animation->nodes[i].name_node);

			if (go != nullptr)
			{
				ComponentTransform* transform = go->GetTransform();
				if (!blend)
				{
					if (resource_animation->nodes[i].CalcCurrentIndex(animation_time * resource_animation->ticks_per_second))
					{
						resource_animation->nodes[i].CalcTransfrom(animation_time * resource_animation->ticks_per_second, interpolation);
						resource_animation->nodes[i].lastTransform.Decompose(transform->local_position, transform->local_rotation, transform->local_scale);
					}
				}
				else
				{

					blend_animation = (ResourceAnimation*)App->resource->Get(blend_id);

					if (blend_animation != nullptr)
					{
						if (blend_animation_time > blend_animation->duration / blend_animation->ticks_per_second && loop)
						{
							blend_animation_time -= blend_animation->duration / blend_animation->ticks_per_second;
						}

						resource_animation->nodes[i].CalcCurrentIndex(animation_time * resource_animation->ticks_per_second);
						blend_animation->nodes[i].CalcCurrentIndex(blend_animation_time * blend_animation->ticks_per_second);

						resource_animation->nodes[i].CalcTransfrom(animation_time * resource_animation->ticks_per_second, interpolation);
						blend_animation->nodes[i].CalcTransfrom(blend_animation_time * blend_animation->ticks_per_second, interpolation);

						resource_animation->nodes[i].AnimBlending(blend_animation->nodes[i].lastTransform, blend_time / total_blend_time);
						resource_animation->nodes[i].lastTransform.Decompose(transform->local_position, transform->local_rotation, transform->local_scale);
					}
				}

				transform->GetTransformMatrix();
				
			}
		}

		if (blend)
		{
			blend_time += dt * speed;
			blend_animation_time += dt * speed;

			if (blend_time > total_blend_time)
			{
				animation_time = blend_animation_time;
				blend_animation_time = 0.0f;
				blend_time = 0.0f;
				resource_animation = blend_animation;
				blend_id = 0;
				blend = false;
				loop = blend_loop;
			}
		}

	}
}

// Save
void ComponentAnimation::Save(const nlohmann::json::iterator& _iterator)
{
	nlohmann::json json;
	if (resource_animation != nullptr && attack_animation != nullptr)
	{
		json = {
		{"type", type},
		{"enabled", enabled},
		{"animation", resource_animation->GetId()},
		{"attack", attack_animation->GetId()},
		{"run", run_animation->GetId()},
		{"idle", idle_animation->GetId()},
		};
	}
	else
	{
		json = {
		{"type", type},
		{"enabled", enabled},
		{"animation", 0},
		{"attack", 0},
		{"run", 0},
		{"idle", 0},
		};
	}
	_iterator.value().push_back(json);
}

void ComponentAnimation::Load(const nlohmann::json _json)
{
	type = _json["type"];
	enabled = _json["enabled"];
	if (_json["animation"] != 0)
		resource_animation = (ResourceAnimation*)App->resource->Get(_json["animation"]);
	if (_json["attack"] != 0)
		attack_animation = (ResourceAnimation*)App->resource->Get(_json["attack"]);
	if (_json["run"] != 0)
		run_animation = (ResourceAnimation*)App->resource->Get(_json["run"]);
	if (_json["idle"] != 0)
		idle_animation = (ResourceAnimation*)App->resource->Get(_json["idle"]);
}

