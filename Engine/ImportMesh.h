#ifndef __ImportMesh_H_
#define __ImportMesh_H_

#include "Importer.h"

struct aiScene;
struct aiMesh;

class ImportMesh : public Importer
{
public:
	ImportMesh();
	~ImportMesh();

	bool Init();
	bool CleanUp();

	void Import(const aiScene* scene, const aiMesh* ai_mesh, GameObject* _object);
	bool Save(ComponentMesh* mesh);
	void Load(ComponentMesh* mesh);

private:
	void GLBuffer(ComponentMesh *mesh);
};

#endif // !__ImportMesh_H_