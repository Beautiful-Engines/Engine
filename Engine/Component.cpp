#include "GameObject.h"
#include "Component.h"


Component::Component(GameObject* _my_game_object, ComponentType _type)
{
	if (_my_game_object != nullptr)
	{
		my_game_object = _my_game_object;
		type = _type;
		_my_game_object->AddComponent(this);
	}
}

Component::~Component()
{
}

void Component::Update(float dt)
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

void Component::Save(const nlohmann::json::iterator& _iterator)
{

}

void Component::Load(const nlohmann::json _json)
{

}

const ComponentType Component::GetType() const
{
	return type;
}

GameObject* Component::GetMyGameObject()
{
	return my_game_object;
}