#include "Application.h"
#include "CustomMesh.h"
#include "ModuleRenderer3D.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h" 
#include "SDL/include/SDL_config.h"

#include "ModuleImport.h"

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
		CustomMesh *mymesh = new CustomMesh();

		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			ai_mesh = scene->mMeshes[i];

			//copy vertices
			mymesh->n_vertices = ai_mesh->mNumVertices;
			mymesh->vertices = new float[mymesh->n_vertices * 3];
			memcpy(mymesh->vertices, ai_mesh->mVertices, sizeof(float) * mymesh->n_vertices * 3);
			LOG("New mesh with %d vertices", mymesh->n_vertices);

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
				LOG("New mesh with %i faces", mymesh->n_indexes / 3);
			}

			if (ai_mesh->HasNormals()) {
				mymesh->normals = new aiVector3D[mymesh->n_vertices];
				memcpy(mymesh->normals, ai_mesh->mNormals, sizeof(aiVector3D) * mymesh->n_vertices);

				mymesh->face_center_point = new float[ai_mesh->mNumFaces * 3];
				mymesh->face_normal = new float[ai_mesh->mNumFaces * 3];

				for (uint i = 0; i < mymesh->n_indexes; i += 3)
				{
					uint aux2 = i + 3;
					uint aux3 = i + 6;

					vec3 x0(mymesh->vertices[i], mymesh->vertices[i + 1], mymesh->vertices[i + 2]);
					vec3 x1(mymesh->vertices[aux2], mymesh->vertices[aux2 + 1], mymesh->vertices[aux2 + 2]);
					vec3 x2(mymesh->vertices[aux3], mymesh->vertices[aux3 + 1], mymesh->vertices[aux3 + 2]);

					vec3 v0 = x1 - x0;
					vec3 v1 = x2 - x0;
					vec3 n = cross(v1, v0);

					vec3 normalized = normalize(n);

					mymesh->face_center_point[i] = (x0.x + x1.x + x2.x) / 3;
					mymesh->face_center_point[i + 1] = (x0.y + x1.y + x2.y) / 3;
					mymesh->face_center_point[i + 2] = (x0.z + x1.z + x2.z) / 3;

					mymesh->face_normal[i] = normalized.x;
					mymesh->face_normal[i + 1] = normalized.y;
					mymesh->face_normal[i + 2] = normalized.z;
				}
			}

			App->renderer3D->GLBuffer(mymesh);
			ai_mesh = nullptr;
		}

		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", aiGetErrorString());
		ret = false;
	}

	return ret;
}


