#include "Application.h"
#include "ModuleResource.h"
#include "ModuleScene.h"
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
	/*if (!bones_loaded)
	{
		FillBones(this->GetMyGameObject()->GetId());
		bones_loaded = true;
	}*/

	if (resource_animation != nullptr)
	{
		if (!pause)
			animation_time += dt * speed;

		if (animation_time > resource_animation->duration / resource_animation->ticks_per_second && loop)
		{
			animation_time -= resource_animation->duration / resource_animation->ticks_per_second;
		}

		for (int i = 0; i < resource_animation->num_channels; i++)
		{
			GameObject* go = App->scene->GetGameObjectByName(resource_animation->nodes[i].name_node);

			if (go != nullptr)
			{
				ComponentTransform* transform = go->GetTransform();
				if (!blend || !smooth)
				{
					if (resource_animation->nodes[i].CalcCurrentIndex(animation_time * resource_animation->ticks_per_second, play))
					{
						resource_animation->nodes[i].CalcTransfrom(animation_time * resource_animation->ticks_per_second, interpolation);
						resource_animation->nodes[i].lastTransform.Decompose(transform->local_position, transform->local_rotation, transform->local_scale);
					}
				}
				else
				{

					ResourceAnimation* blend_animation = (ResourceAnimation*)App->resource->Get(blend_id);

					if (blend_animation != nullptr)
					{
						if (blend_animation_time > blend_animation->duration / blend_animation->ticks_per_second && loop)
						{
							blend_animation_time -= blend_animation->duration / blend_animation->ticks_per_second;
						}

						resource_animation->nodes[i].CalcCurrentIndex(animation_time * resource_animation->ticks_per_second, play);
						blend_animation->nodes[i].CalcCurrentIndex(blend_animation_time * blend_animation->ticks_per_second, play);

						resource_animation->nodes[i].CalcTransfrom(animation_time * resource_animation->ticks_per_second, interpolation);
						blend_animation->nodes[i].CalcTransfrom(blend_animation_time * blend_animation->ticks_per_second, interpolation);

						resource_animation->nodes[i].AnimBlending(blend_animation->nodes[i].lastTransform, blend_time / total_blend_time);
						resource_animation->nodes[i].lastTransform.Decompose(transform->local_position, transform->local_rotation, transform->local_scale);
					}
				}

				go->GetTransform()->GetTransformMatrix();
				
			}
		}

		if (blend && smooth)
		{
			blend_time += dt * speed;
			blend_animation_time += dt * speed;

			if (blend_time > total_blend_time)
			{
				FillBones(blend_id);
				animation_time = blend_animation_time;
				blend_animation_time = 0.0f;
				blend_time = 0.0f;
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
	if (resource_animation != nullptr)
	{
		json = {
		{"type", type},
		{"enabled", enabled},
		{"animation", resource_animation->GetId()},
		};
	}
	else
	{
		json = {
		{"type", type},
		{"enabled", enabled},
		{"animation", 0},
		};
	}
	bones_loaded = false;
	_iterator.value().push_back(json);
}

void ComponentAnimation::Load(const nlohmann::json _json)
{
	type = _json["type"];
	enabled = _json["enabled"];
	if (_json["animation"] != 0)
		resource_animation = (ResourceAnimation*)App->resource->Get(_json["animation"]);
}

void ComponentAnimation::FillBones(uint _id)
{
	
	std::vector<GameObject*> go = App->scene->GetGameObject(_id)->GetChildren();
	std::vector<GameObject*>::iterator iterator_go = go.begin();

	for (; iterator_go != go.end(); ++iterator_go) 
	{
		if ((*iterator_go) != nullptr && (*iterator_go)->GetBone() != nullptr)
		{
			bones_go.push_back((*iterator_go)->GetId());
		}
		if ((*iterator_go)->GetChildren().size() > 0)
		{
			for (uint i = 0; i < (*iterator_go)->GetChildren().size(); i++)
			{
				FillBones((*iterator_go)->GetChildren()[i]->GetId());
			}
				
		}
	}

}
