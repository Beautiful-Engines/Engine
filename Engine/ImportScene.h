#ifndef __ImportScene_H_
#define __ImportScene_H_

#include "Importer.h"

struct aiNode;
struct aiScene;

class ImportScene : public Importer
{
public:
	ImportScene();
	~ImportScene();

	bool Init();
	bool CleanUp();

	bool LoadFBX(const char* _path);

private:
	void LoadNode(const aiNode* _node, const aiScene* _scene, GameObject* _object);
};

#endif // !__ImportScene_H_