#include "Application.h"
#include "ComponentTexture.h"
#include "ModuleRenderer3D.h"
#include "Primitive.h"
#include "GameObject.h"


GameObject::GameObject()
{
	new ComponentTransform(this);
	id = App->GenerateNewId();
}

GameObject::~GameObject()
{
	if (components.size() == 0)
	{
		for (auto iterator_components = components.begin(); iterator_components < components.end(); iterator_components++) {
			if (*iterator_components != nullptr)
			{
				RELEASE(*iterator_components);
				iterator_components = components.erase(iterator_components);
				if (components.size() == 0)
					break;
			}
		}
	}
	components.clear();

	for (auto iterator_children = children.begin(); iterator_children < children.end(); ++iterator_children) {
		if (*iterator_children != nullptr)
		{
			RELEASE(*iterator_children);
			iterator_children = children.erase(iterator_children);
		}
	}
	children.clear();

}

void GameObject::Update()
{
	std::vector<Component*>::iterator iterator_component = components.begin();
	
	for (; iterator_component != components.end(); ++iterator_component) {
		if (*iterator_component != nullptr && (*iterator_component)->GetType() == ComponentType::MESH)
		{
			(*iterator_component)->Update();
		}
	}
	if (GetCamera()) {
		if (GetCamera()->active == false)
		{
			GetCamera()->UpdateFrustumTransform();
			GetCamera()->DrawFrustum();
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
	float3 x = App->renderer3D->camera->frustum.pos;
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

// Save
void GameObject::Save(const nlohmann::json::iterator& _iterator)
{
	LOG("%s saved", name.c_str());
	nlohmann::json json = {
		{"UID", id},
		{"Name", name},
		{"ParentUID", GetParent() ? GetParent()->GetId() : 0},
		{"Enable", enabled},
		{"Components",nlohmann::json::array()}
	};

	//Save Components
	for (int i = 0; i < components.size(); ++i)
	{
		components[i]->Save(json.find("Components"));
	}

	_iterator.value().push_back(json);

	// Recursive Save Children
	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->Save(_iterator);
	}
}

// Components
Component* GameObject::CreateComponent(ComponentType type)
{
	Component *component = new Component(this, type);
	return component;
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
}

// Getters and Setters

// Id
const uint GameObject::GetId() const
{
	return id;
}

// IdNode
const uint GameObject::GetIdNode() const
{
	return id_node;
}

const uint GameObject::GetIdNodeParent() const
{
	return id_node_parent;
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

GameObject* GameObject::GetParent()
{
	return parent;
}

void GameObject::SetIdNode(uint _id_node)
{
	id_node = _id_node;
}

void GameObject::SetIdNodeParent(uint _id_node_parent)
{
	id_node_parent = _id_node_parent;
}

void GameObject::SetParent(GameObject* _game_object)
{
	parent = _game_object;
	_game_object->children.push_back(this);
}

const std::vector<GameObject*> GameObject::GetChildren() const
{
	return children;
}

const bool GameObject::IsChild(GameObject * _game_object) const
{
	bool ret = false;

	for (int i = 0; i < children.size() && !ret; i++)
	{
		if (children[i] == _game_object)
			return true;

		ret = children[i]->IsChild(_game_object);
	}

	return ret;
}

void GameObject::DeleteChild(GameObject* _game_object)
{
	std::vector<GameObject*>::iterator iterator_go = children.begin();
	for (; iterator_go != children.end();) {
		if (*iterator_go != nullptr && (*iterator_go) == _game_object)
		{
			iterator_go = children.erase(iterator_go);
		}
		else
		{
			iterator_go++;
		}
	}
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
	return nullptr;
}


ComponentCamera * GameObject::GetCamera() const
{
	std::string x = GetName();
	for (uint i = 0; i < this->GetComponents().size(); ++i)
	{
		if (this->GetComponents()[i]->GetType() == ComponentType::CAMERA)
		{
			return (ComponentCamera*)this->GetComponents()[i];
		}
	}
	return nullptr;
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

void GameObject :: UpdateBB()
{
	if (GetMesh()->GetResourceMesh())
	{
		obb = GetMesh()->GetBB();
		obb.Transform(GetTransform()->transform_matrix);

		abb.SetNegativeInfinity();
		abb.Enclose(obb);
	}
}