#include "Application.h"
#include "GameObject.h"
#include "ComponentMesh.h"

GameObject::GameObject()
{
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
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->IsEnabled())
			components[i]->Update();
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
const const char* GameObject::GetName() const
{
	return name;
}
void GameObject::SetName(const char* _name) 
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

const std::vector<GameObject*> GameObject::GetChildren() const
{
	return children;
}

const bool GameObject::IsFocused() const
{
	return focused;
}