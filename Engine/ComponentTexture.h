#ifndef __ComponentTexture_H__
#define __ComponentTexture_H__

#include "Component.h"
class ResourceMesh;

class ComponentTexture : public Component
{
public:
	ComponentTexture(GameObject* _game_object);
	~ComponentTexture();

	void Save(const nlohmann::json::iterator& _iterator);
	void Load(const nlohmann::json _json);

	void DrawTexture(ResourceMesh* _resource_mesh);

public:
	uint id_texture = 0u;  
	uint width = 0;
	uint height = 0;
	std::string path;

};

#endif // !__ComponentTexture_H__