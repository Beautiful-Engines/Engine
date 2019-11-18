#ifndef __ImportTexture_H_
#define __ImportTexture_H_

class ResourceMesh;
class ResourceTexture;
struct aiScene;
struct aiMesh;

class ImportTexture
{
public:
	ImportTexture();
	~ImportTexture();

	bool Init();
	bool CleanUp();

	bool Import(const char* _import_file, std::string& _output_file);
	uint LoadTexture(const char* _path, ResourceMesh* _resource_mesh = nullptr, ResourceTexture* _resource_texture = nullptr);
	void DefaultTexture(ResourceMesh* _resource_mesh);
};

#endif // !__ImportTexture_H_