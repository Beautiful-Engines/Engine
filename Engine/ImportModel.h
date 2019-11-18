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

private:
	ResourceModel::ModelNode ImportNode(const aiNode* _node, const aiScene* _scene, ResourceMesh* _object);
};

#endif // !__ImportScene_H_