#ifndef __GameObject_H__
#define __GameObject_H__

#include <vector>
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "MathGeoLib\include\Geometry\AABB.h"
#include "MathGeoLib\include\Geometry\OBB.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Update();

	void Enable();
	void Disable();
	const bool IsEnabled() const;

	Component* CreateComponent(ComponentType type);
	void AddComponent(Component* component);

	const std::string GetName() const;
	void SetName(std::string _name);
	const std::vector<Component*> GetComponents() const;

	const GameObject* GetParent() const;
	void SetParent(GameObject* _game_object);
	const std::vector<GameObject*> GetChildren() const;
	void DeleteChildren(GameObject* _game_object);

	ComponentTransform* GetTransform() const;

	ComponentMesh* GetMesh();

	const bool IsFocused() const;
	void SetFocus(bool focus);

	void UpdateBB();

	bool hide = false;
	AABB abb;
	OBB obb;

private:
	bool enabled = true;

	std::string name;
	std::vector<Component*> components;

	GameObject* parent = nullptr;
	std::vector<GameObject*> children;

	bool focus = false;

};

#endif // !__GameObject_H__