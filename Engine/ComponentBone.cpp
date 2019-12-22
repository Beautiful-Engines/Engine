#include "Application.h"
#include "ModuleResource.h"
#include "ResourceBone.h"
#include "ComponentBone.h"


ComponentBone::ComponentBone(GameObject* _game_object) : Component(_game_object, ComponentType::BONE)
{
}

ComponentBone::~ComponentBone()
{
}

// Save
void ComponentBone::Save(const nlohmann::json::iterator& _iterator)
{
	nlohmann::json json;
	if (resource_bone != nullptr)
	{
		json = {
		{"type", type},
		{"enabled", enabled},
		{"bone", resource_bone->GetId()},
		};
	}
	else
	{
		json = {
		{"type", type},
		{"enabled", enabled},
		{"bone", 0},
		};
	}

	_iterator.value().push_back(json);
}

void ComponentBone::Load(const nlohmann::json _json)
{
	type = _json["type"];
	enabled = _json["enabled"];
	if (_json["bone"] != 0)
		resource_bone = (ResourceBone*)App->resource->Get(_json["bone"]);
}
