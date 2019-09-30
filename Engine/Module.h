#ifndef __Module_H__
#define __Module_H__

#include "nlohmann\json.hpp"

class Module
{
private:
	bool enabled;

public:
	std::string name;

public:

	Module(bool start_enabled = true) {}

	virtual ~Module()
	{}

	virtual bool Init()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	//Load and Save
	virtual bool LoadDefault(nlohmann::json &load_default_json)
	{
		return true;
	}

	virtual bool Load(nlohmann::json &load_json)
	{
		return true;
	}

	virtual bool Save(nlohmann::json &save_json)
	{
		return true;
	}
};

#endif // !__Module_H__