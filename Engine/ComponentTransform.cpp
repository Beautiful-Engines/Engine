#include "ComponentTransform.h"



ComponentTransform::ComponentTransform(GameObject* _game_object) : Component(_game_object, ComponentType::TRANSFORM)
{
	position = { 0.0f, 0.0f, 0.0f };
	rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
	scale = { 1.0f, 1.0f, 1.0f };

	local_position = { 0.0f, 0.0f, 0.0f };
	local_rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
	local_scale = { 1.0f, 1.0f, 1.0f };
}


ComponentTransform::~ComponentTransform()
{
}
