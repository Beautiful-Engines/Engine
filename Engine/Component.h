#ifndef __Component_H__
#define __Component_H__

class GameObject;

#include "Assimp/include/vector3.h"
#include "Globals.h"

enum class ComponentType {
	MESH,
	MATERIAL,
	TRANSFORM,
	OTHER
};

class Component
{
public:
	Component(GameObject* my_game_object, ComponentType _type);
	virtual ~Component();

	virtual void Update();

	virtual void Enable();
	virtual void Disable();
	const bool IsEnabled() const;

	const ComponentType GetType() const;
	const GameObject* GetMyGameObject() const;

protected:
	bool enabled = true;
	ComponentType type = ComponentType::OTHER;
	GameObject* my_game_object = nullptr;
};

#endif // !__Component_H__