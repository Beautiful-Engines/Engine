#ifndef __Component_H__
#define __Component_H__

class GameObject;

#include "Assimp/include/vector3.h"
#include "Globals.h"
#include "nlohmann/json.hpp"

enum class ComponentType {
	MESH,
	TEXTURE,
	ANIMATION,
	BONE,
	TRANSFORM,
	CAMERA,
	OTHER
};

class Component
{
public:
	Component(GameObject* my_game_object, ComponentType _type);
	virtual ~Component();

	virtual void Update(float dt);

	virtual void Enable();
	virtual void Disable();
	const bool IsEnabled() const;

	virtual void Save(const nlohmann::json::iterator& _iterator);
	virtual void Load(const nlohmann::json _json);

	const ComponentType GetType() const;
	GameObject* GetMyGameObject();


protected:
	bool enabled = true;
	ComponentType type = ComponentType::OTHER;
	GameObject* my_game_object = nullptr;

};

#endif // !__Component_H__