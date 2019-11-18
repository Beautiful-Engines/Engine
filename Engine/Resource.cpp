#include "Application.h"
#include "Resource.h"


Resource::Resource(RESOURCE_TYPE _type, uint UID)
{
	if (UID == 0)
		id = App->GenerateNewId();
	else
		id = UID;
	type = _type;
}

Resource::~Resource()
{
}

Resource::RESOURCE_TYPE Resource::GetType() const
{
	return type;
}

uint Resource::GetId() const
{
	return id;
}

void Resource::SetId(uint UID)
{
	id = UID;
}

const char* Resource::GetName() const
{
	return name.c_str();
}

void Resource::SetName(std::string _name)
{
	name = _name;
}

const char* Resource::GetFile() const
{
	return file.c_str();
}

void Resource::SetFile(std::string _file)
{
	file = _file;
}
