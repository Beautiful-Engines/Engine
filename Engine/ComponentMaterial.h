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
	int width = -1;
	int height = -1;
	const char* path;

};

#endif // !__ComponentMesh_H__