#include "Application.h"
#include "ModuleResource.h"
#include "ResourceBone.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "ComponentBone.h"

#include "glew/glew.h"


ComponentBone::ComponentBone(GameObject* _game_object) : Component(_game_object, ComponentType::BONE)
{
}

ComponentBone::~ComponentBone()
{
}

void ComponentBone::DebugDrawBones()
{
	if (debug)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glLineWidth(2.f);

		ComponentTransform* transform = (ComponentTransform*)this->GetMyGameObject()->GetTransform();
		float3 position;
		Quat rot;
		float3 scale;
		
		transform->GetTransformMatrix().Decompose(position, rot, scale);

		auto children = this->GetMyGameObject()->GetChildren();
		for (auto child : children)
		{
			ComponentTransform* transform_child = (ComponentTransform*)this->GetMyGameObject()->GetTransform();
			float3 position_child;
			Quat rot_child;
			float3 scale_child;

			transform_child->GetTransformMatrix().Decompose(position_child, rot_child, scale_child);
			glBegin(GL_LINES);
			glVertex3f(position.x, position.y, position.z);
			glVertex3f(position_child.x, position_child.y, position_child.z);
			glEnd();
		}
	}
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
