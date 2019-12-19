#include "Application.h"

#include "ResourceBone.h"

ResourceBone::ResourceBone(uint UID) :Resource(Resource::RESOURCE_TYPE::RESOURCE_BONE, UID)
{
}

ResourceBone::~ResourceBone()
{
	RELEASE_ARRAY(weights);
}
