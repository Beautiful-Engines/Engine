#include "Application.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "Primitive.h"
#include "GameObject.h"


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
	std::vector<Component*>::iterator iterator_component = components.begin();
	
	
	for (; iterator_component != components.end(); ++iterator_component) {
		if (*iterator_component != nullptr && (*iterator_component)->GetType() == ComponentType::MESH)
		{
			ComponentMesh* mesh;
			mesh = (ComponentMesh*)*iterator_component;
			mesh->Update();
		}
		if (*iterator_component != nullptr && (*iterator_component)->GetType() == ComponentType::MATERIAL)
		{
			ComponentMaterial* material;
			material = (ComponentMaterial*)*iterator_component;
			material->Texture();
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

void GameObject::SetParent(GameObject* game_object)
{
	parent = game_object;
}

const std::vector<GameObject*> GameObject::GetChildren() const
{
	return children;
}

const bool GameObject::IsFocused() const
{
	return focused;
}