#ifndef _ResourceTexture_H_
#define _ResourceTexture_H_

#include "Resource.h"

class ResourceTexture : public Resource
{
public:
	ResourceTexture(uint UID = 0);
	~ResourceTexture();
	
	uint id_texture = 0u;
	uint width, height;
	std::string path = "";
};


#endif // !_ResourceTexture_H_