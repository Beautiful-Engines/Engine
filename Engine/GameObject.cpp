#include "Application.h"
#include "ComponentMaterial.h"
#include "Primitive.h"
#include "GameObject.h"


GameObject::GameObject()
{
	new ComponentTransform(this);
}

GameObject::~GameObject()
{
	std::vector<Component*>::iterator iterator_components = components.begin();
	for (; iterator_components != components.end(); ++iterator_components) {
		if (*iterator_components != nullptr) {
			delete* iterator_components;
			*iterator_components = nullptr;
		}
	}

	std::vector<GameObject*>::iterator iterator_children = children.begin();
	for (; iterator_children != children.end(); ++iterator_children) {
		if (*iterator_children != nullptr) {
			delete* iterator_children;
			*iterator_children = nullptr;
		}
	}
}

void GameObject::Update()
{
	std::vector<Component*>::iterator iterator_component = components.begin();
	
	for (; iterator_component != components.end(); ++iterator_component) {
		if (*iterator_component != nullptr && (*iterator_component)->GetType() == ComponentType::MESH)
		{
			ComponentMesh* mesh = (ComponentMesh*)*iterator_component;
			mesh->Update();
		}
	}

	if (children.size() > 0)
	{
		std::vector<GameObject*>::iterator iterator_go = children.begin();

		for (; iterator_go != children.end(); ++iterator_go) {
			if (*iterator_go != nullptr && (*iterator_go)->IsEnabled())
			{
				(*iterator_go)->Update();
			}
		}
	}
}

void GameObject::Enable()
{
	enabled = true;
}

void GameObject::Disable()
{
	enabled = false;
}

const bool GameObject::IsEnabled() const
{
	return enabled;
}

// Components
Component* GameObject::CreateComponent(ComponentType type)
{
	Component *component = new Component(this, type);
	components.push_back(component);
	return component;
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
}

// Name
const std::string GameObject::GetName() const
{
	return name;
}
void GameObject::SetName(std::string _name)
{
	name = _name;
}

// Getters
const std::vector<Component*> GameObject::GetComponents() const
{
	return components;
}

const GameObject* GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetParent(GameObject* game_object)
{
	parent = game_object;
	game_object->children.push_back(this);
}

const std::vector<GameObject*> GameObject::GetChildren() const
{
	return children;
}

ComponentTransform * GameObject::GetTransform() const
{
	for (uint i = 0; i < this->GetComponents().size(); ++i)
	{
		if (this->GetComponents()[i]->GetType() == ComponentType::TRANSFORM)
		{
			return (ComponentTransform*)this->GetComponents()[i];
		}
	}
}

ComponentMesh * GameObject::GetMesh()
{
	for (uint i = 0; i < this->GetComponents().size(); ++i)
	{
		if (this->GetComponents()[i]->GetType() == ComponentType::MESH)
		{
			return (ComponentMesh*)this->GetComponents()[i];
		}
	}
	return false;
}

const bool GameObject::IsFocused() const
{
	return focus;
}

void GameObject::SetFocus(bool _focus)
{
	focus = _focus;
}