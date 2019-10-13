#include "Application.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "ModuleScene.h"
#include "ModuleImport.h"

#include "glmath.h"
#include "glew\glew.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h" 

#pragma comment (lib, "Assimp/libx86/assimp.lib")


ModuleImport::ModuleImport(bool start_enabled) : Module(start_enabled)
{
	name = "Import";
}


ModuleImport::~ModuleImport()
{
}

bool ModuleImport::Start()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	return true;
}

bool ModuleImport::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

bool ModuleImport::LoadFile(const char* _path)
{
	bool ret = true;

	const aiScene * scene = aiImportFile(_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		aiMesh *ai_mesh = nullptr;
		GameObject *go = new GameObject();
		
		ComponentMesh *mymesh = new ComponentMesh(go);
		std::string name_mesh = _path;
		uint pos = name_mesh.find("\\");
		
		while (pos > 0 && pos < 1000) {
			name_mesh = name_mesh.substr(pos + 1);
			pos = name_mesh.find("\\");
		}

		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			ai_mesh = scene->mMeshes[i];

			//copy vertices
			mymesh->n_vertices = ai_mesh->mNumVertices;
			mymesh->vertices = new float[mymesh->n_vertices * 3];
			memcpy(mymesh->vertices, ai_mesh->mVertices, sizeof(float) * mymesh->n_vertices * 3);

			if (ai_mesh->HasFaces())
			{
				mymesh->n_indexes = ai_mesh->mNumFaces * 3;
				mymesh->indexes = new uint[mymesh->n_indexes];

				for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
				{
					if (ai_mesh->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&mymesh->indexes[j * 3], ai_mesh->mFaces[j].mIndices, sizeof(uint) * 3);
					}
				}
				LOG("New mesh %s, with %i vertices and %i faces has been added", name_mesh.c_str(), mymesh->n_vertices, mymesh->n_indexes / 3);
			}

			// Normals
			if (ai_mesh->HasNormals()) 
			{
				mymesh->normals = new aiVector3D[mymesh->n_vertices];
				memcpy(mymesh->normals, ai_mesh->mNormals, sizeof(aiVector3D) * mymesh->n_vertices);

				mymesh->face_center_point = new float[ai_mesh->mNumFaces * 3];
				mymesh->face_normal = new float[ai_mesh->mNumFaces * 3];

				for (uint i = 0; i < mymesh->n_indexes; i += 3)
				{
					uint index = mymesh->indexes[i];
					vec3 x0(mymesh->vertices[index * 3], mymesh->vertices[index * 3 + 1], mymesh->vertices[index * 3 + 2]);
					index = mymesh->indexes[i + 1];
					vec3 x1(mymesh->vertices[index * 3], mymesh->vertices[index * 3 + 1], mymesh->vertices[index * 3 + 2]);
					index = mymesh->indexes[i + 2];
					vec3 x2(mymesh->vertices[index * 3], mymesh->vertices[index * 3 + 1], mymesh->vertices[index * 3 + 2]);

					vec3 v0 = x1 - x0;
					vec3 v1 = x2 - x0;
					vec3 n = cross(v0, v1);

					vec3 normalized = normalize(n);

					mymesh->face_center_point[i] = (x0.x + x1.x + x2.x) / 3;
					mymesh->face_center_point[i + 1] = (x0.y + x1.y + x2.y) / 3;
					mymesh->face_center_point[i + 2] = (x0.z + x1.z + x2.z) / 3;

					mymesh->face_normal[i] = normalized.x;
					mymesh->face_normal[i + 1] = normalized.y;
					mymesh->face_normal[i + 2] = normalized.z;
				}
			}

			// UVs
			if (ai_mesh->HasTextureCoords(0))
			{
				mymesh->id_uv = ai_mesh->mNumUVComponents[0];
				mymesh->uv_coords = new float[mymesh->n_vertices * mymesh->id_uv];

				for (uint i = 0; i < mymesh->n_vertices; i++)
				{
					memcpy(&mymesh->uv_coords[i * mymesh->id_uv], &ai_mesh->mTextureCoords[0][i], sizeof(float) * mymesh->id_uv);
				}
			}

			GLBuffer(mymesh);

			ai_mesh = nullptr;
		}

		App->scene->AddGameObject(go);
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", aiGetErrorString());
		ret = false;
	}

	return ret;
}

void ModuleImport::GLBuffer(ComponentMesh *mesh)
{
	glGenBuffers(1, &mesh->id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->n_vertices * 3, mesh->vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &mesh->id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->n_indexes, mesh->indexes, GL_STATIC_DRAW);

	//NORMALS
	mesh->face_normal = new float[mesh->n_indexes];
	for (int i = 0; i < mesh->n_indexes; ++i) {
		mesh->face_normal[i] = (mesh->vertices[i] + mesh->vertices[i + 1] + mesh->vertices[i + 2]) / 3;
	}

	if (mesh->normals != nullptr) {
		glGenBuffers(1, &mesh->id_normal);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normal);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->n_vertices * 3, mesh->normals, GL_STATIC_DRAW);
	}

}