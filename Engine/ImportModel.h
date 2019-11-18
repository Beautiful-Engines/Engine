#ifndef __ImportModel_H_
#define __ImportModel_H_

#include "ResourceModel.h"

struct aiNode;
struct aiScene;

class ImportModel
{
public:
	ImportModel();
	~ImportModel();

	bool Init();
	bool CleanUp();

	uint ImportFBX(const char* _path);
	bool LoadModel(ResourceModel* resource);

private:
	ResourceModel::ModelNode ImportNode(const aiNode* _node, const aiScene* _scene, ResourceMesh* _object);
	bool LoadNode(nlohmann::json::iterator it, ResourceModel* _resource);
	void CreateOurModelFile(ResourceModel* _resource);
};

#endif // !__ImportModel_H_