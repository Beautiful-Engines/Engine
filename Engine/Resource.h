#ifndef _Resource_H_
#define _Resource_H_

#include "Globals.h"
#include <string>

class Resource
{
public:
	enum RESOURCE_TYPE {
		RESOURCE_TEXTURE,
		RESOURCE_MESH,
		RESOURCE_MODEL,
		RESOURCE_SCENE,
		RESOURCE_ANIMATION,
		RESOURCE_BONE,
		RESOURCE_GRAPH,
		NONE
	};
public:
	Resource(RESOURCE_TYPE type, uint UID= 0);
	~Resource();
	

	RESOURCE_TYPE GetType() const;
	uint GetId() const;
	void SetId(uint UID);
	const char* GetName() const;
	void SetName(std::string _name);
	const char* GetFile() const;
	void SetFile(std::string _file);
	const uint GetCantities() const;
	void SetCantities(uint _cantity);

protected:
	uint id = 0;
	std::string name = "noname";
	std::string file = "";
	RESOURCE_TYPE type = RESOURCE_TYPE::NONE;
	uint cantity = 0;

};

#endif // !_Resource_H_