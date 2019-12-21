#ifndef _ComponentAnimation_H_
#define _ComponentAnimation_H_

#include "Component.h"

class ResourceAnimation;

class ComponentAnimation :	public Component
{
public:
	ComponentAnimation(GameObject* _game_object);
	~ComponentAnimation();

	void Save(const nlohmann::json::iterator& _iterator);
	void Load(const nlohmann::json _json);

public:
	ResourceAnimation* animation = nullptr;

};

#endif // !_ComponentAnimation_H_
