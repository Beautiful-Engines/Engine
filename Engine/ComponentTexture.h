#ifndef __ComponentTexture_H__
#define __ComponentTexture_H__

#include "Component.h"

class ResourceMesh;
class ResourceTexture;

class ComponentTexture : public Component
{
public:
	ComponentTexture(GameObject* _game_object);
	~ComponentTexture();

	void Save(const nlohmann::json::iterator& _iterator);
	void Load(const nlohmann::json _json);

	void DrawTexture(ResourceMesh* _resource_mesh);

public:
	ResourceTexture* texture = nullptr;
	ResourceTexture* default_texture = nullptr;

	bool checkered= false;

};

#endif // !__ComponentTexture_H__