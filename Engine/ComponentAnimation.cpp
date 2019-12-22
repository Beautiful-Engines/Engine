#include "Application.h"
#include "ModuleResource.h"
#include "ResourceAnimation.h"
#include "ComponentAnimation.h"


ComponentAnimation::ComponentAnimation(GameObject* _game_object) : Component(_game_object, ComponentType::ANIMATION)
{
}

ComponentAnimation::~ComponentAnimation()
{
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

	_iterator.value().push_back(json);
}

void ComponentAnimation::Load(const nlohmann::json _json)
{
	type = _json["type"];
	enabled = _json["enabled"];
	if (_json["animation"] != 0)
		resource_animation = (ResourceAnimation*)App->resource->Get(_json["animation"]);
}
