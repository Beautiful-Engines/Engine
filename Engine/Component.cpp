#include "GameObject.h"
#include "Component.h"


Component::Component(GameObject* _my_game_object, ComponentType _type)
{
	my_game_object = _my_game_object;
	type = _type;
	_my_game_object->AddComponent(this);
}

Component::~Component()
{
}

void Component::Update()
{
}

void Component::Enable()
{
	enabled = true;
}

void Component::Disable()
{
	enabled = false;
}

const bool Component::IsEnabled() const
{
	return enabled;
}

const ComponentType Component::GetType() const
{
	return type;
}

const GameObject* Component::GetMyGameObject() const
{
	return my_game_object;
}