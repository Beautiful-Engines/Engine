#ifndef __ImportMesh_H_
#define __ImportMesh_H_

struct aiScene;
struct aiMesh;
class GameObject;
class ResourceMesh;

class ImportMesh 
{
public:
	ImportMesh();
	~ImportMesh();

	bool Init();
	bool CleanUp();

	void Import(const aiScene* scene, const aiMesh* ai_mesh, ResourceMesh* _resource_mesh);
	bool CreateOurMesh(ResourceMesh* mesh);
	void LoadMeshFromResource(ResourceMesh* mesh);

private:
	void GLBuffer(ResourceMesh *mesh);
};

#endif // !__ImportMesh_H_