#ifndef _ResourceModel_H_
#define _ResourceModel_H_

#include "Resource.h"
#include "MathGeoLib\include\Math\MathAll.h"

class ResourceModel : public Resource
{
public:
	struct ModelNode
	{
		std::string name;
		uint id;
		uint parent;

		float3 position;
		Quat rotation;
		float3 scale;
		
		uint mesh = 0;
		uint texture = 0;
		uint bone = 0;
	};
public:
	ResourceModel(uint UID = 0);
	~ResourceModel();

	std::vector<ModelNode> nodes;
	uint animation = 0;
};


#endif // !_ResourceModel_H_