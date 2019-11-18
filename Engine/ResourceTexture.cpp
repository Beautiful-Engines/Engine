#include "ResourceTexture.h"

#include "glew\glew.h"



ResourceTexture::ResourceTexture(uint UID) : Resource(Resource::RESOURCE_TYPE::RESOURCE_TEXTURE, UID)
{
}

ResourceTexture::~ResourceTexture()
{
	glDeleteBuffers(1, &id_texture);
	id_texture = 0;
}