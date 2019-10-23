#ifndef __ComponentMaterial_H__
#define __ComponentMaterial_H__

#include "Component.h"
class ComponentMesh;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* _game_object);
	~ComponentMaterial();

	void Texture(ComponentMesh* _component_mesh);

public:
	uint id_texture = 0u;  
	uint width = 0;
	uint height = 0;
	bool checkered = false;
	std::string path;

};

#endif // !__ComponentMaterial_H__