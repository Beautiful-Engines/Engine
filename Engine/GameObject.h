#ifndef __GameObject_H__
#define __GameObject_H__

#include <vector>
#include "Component.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	void Update();

	void Enable();
	void Disable();
	const bool IsEnabled() const;

	Component* CreateComponent(ComponentType type);
	void AddComponent(Component* component);

	const const char* GetName() const;
	void SetName(const char* _name);
	const std::vector<Component*> GetComponents() const;

	const GameObject* GetParent() const;
	const std::vector<GameObject*> GetChildren() const;

	const bool IsFocused() const;


private:
	bool enabled = true;

	const char* name;
	std::vector<Component*> components;

	GameObject* parent = nullptr;
	std::vector<GameObject*> children;

	bool focused = false;
};

#endif // !__GameObject_H__