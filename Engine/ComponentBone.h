#ifndef _ComponentBone_H_
#define _ComponentBone_H_

#include "Component.h"

class ResourceBone;

class ComponentBone : public Component
{
public:
	ComponentBone(GameObject* _game_object);
	~ComponentBone();

	void Save(const nlohmann::json::iterator& _iterator);
	void Load(const nlohmann::json _json);

public:
	ResourceBone* resource_bone = nullptr;

};

#endif // !_ComponentBone_H_