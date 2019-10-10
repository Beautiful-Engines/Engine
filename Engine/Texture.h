#ifndef __Texture_H__
#define __Texture_H__

#include "Globals.h"

class aiMaterial;
enum aiTextureType;

class Texture
{
public:
	Texture();
	~Texture();

	void LoadTexture(aiMaterial* _material, aiTextureType _tex_type);

public:
	uint id_texture;
};

#endif // !__Texture_H__