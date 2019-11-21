#ifndef __GameObject_H__
#define __GameObject_H__

#include <vector>
#include <algorithm>
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "MathGeoLib\include\Geometry\AABB.h"
#include "MathGeoLib\include\Geometry\OBB.h"
#include "nlohmann/json.hpp"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Update();

	void Enable();
	void Disable();
	const bool IsEnabled() const;

	void Save(const nlohmann::json::iterator& _iterator);

	Component* CreateComponent(ComponentType type);
	void AddComponent(Component* component);

	const uint GetId() const;
	const uint GetIdNode() const;
	const uint GetIdNodeParent() const;
	const std::string GetName() const;
	void SetName(std::string _name);
	const std::vector<Component*> GetComponents() const;
	GameObject* GetParent();
	
	void SetId(uint _id);
	void SetIdNode(uint _id_node);
	void SetIdNodeParent(uint _id_node_parent);
	void SetParent(GameObject* _game_object);
	const std::vector<GameObject*> GetChildren() const;
	void DeleteChild(GameObject* _game_object);
	const bool GameObject::IsChild(GameObject * _game_object) const;

	const bool IsLastChild(GameObject * _game_object) const;

	ComponentTransform* GetTransform() const;

	ComponentMesh* GetMesh();

	ComponentCamera* GetCamera() const;

	const bool IsFocused() const;
	void SetFocus(bool focus);

	void UpdateBB();

	bool hide = false;
	AABB abb;
	OBB obb;

private:
	bool enabled = true;

	uint id = 0u;
	uint id_node = 0u;
	uint id_node_parent = 0u;
	std::string name;
	std::vector<Component*> components;

	GameObject* parent = nullptr;
	std::vector<GameObject*> children;

	bool focus = false;

};

#endif // !__GameObject_H__