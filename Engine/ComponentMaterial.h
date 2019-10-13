#ifndef __ComponentMaterial_H__
#define __ComponentMaterial_H__

#include "Component.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* _game_object);
	~ComponentMaterial();

	void Texture();

public:
	uint id_texture = 0u;  
	uint indexes = 0u;

};

#endif // !__ComponentMesh_H__