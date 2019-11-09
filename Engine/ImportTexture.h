#ifndef __ImportTexture_H_
#define __ImportTexture_H_

#include "Importer.h"

class GameObject;
struct aiScene;
struct aiMesh;

class ImportTexture : public Importer
{
public:
	ImportTexture();
	~ImportTexture();

	bool Init();
	bool CleanUp();

	bool Import(const char* _import_file, std::string& _output_file);
	bool LoadTexture(const char* _path, GameObject* go_fromfbx = nullptr);
	void DefaultTexture(GameObject* go_texturedefault);

	std::string final_path = "";
};

#endif // !__ImportTexture_H_