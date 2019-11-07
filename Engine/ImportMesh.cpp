#include "Application.h"
#include "ModuleFileSystem.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "ComponentMesh.h"
#include "GameObject.h"
#include "ImportMesh.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ImportMesh::ImportMesh()
{
}


ImportMesh::~ImportMesh()
{
}

bool ImportMesh::Init()
{
	return true;
}

bool ImportMesh::CleanUp()
{
	return true;
}

//ComponentMesh* ImportMesh::Load(const char * exported_file)
//{
//	
//}
//
//ComponentMesh* ImportMesh::Import(const aiScene* scene, const aiMesh* mesh)
//{
//	
//}

bool ImportMesh::Save(ComponentMesh* mesh)
{
	// amount of indices / vertices / colors / normals / texture_coords / AABB
	uint ranges[4] = { mesh->n_indexes, mesh->n_vertices, mesh->n_normals, mesh->n_uv };
	uint size = sizeof(ranges) + sizeof(uint) * mesh->n_indexes + sizeof(float3) * mesh->n_vertices + sizeof(float3) * mesh->n_normals + sizeof(float2) * mesh->n_uv;

	// Allocate
	char* data = new char[size];
	char* cursor = data;

	// First store ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	// Store indexes
	bytes = sizeof(uint) * mesh->n_indexes;
	memcpy(cursor, mesh->indexes, bytes);
	cursor += bytes;

	// Store vertices
	bytes = sizeof(float3) * mesh->n_vertices;
	memcpy(cursor, mesh->vertices, bytes);
	cursor += bytes;

	if (mesh->n_normals > 0)
	{
		//Store normals
		bytes = sizeof(float3) * mesh->n_normals;
		memcpy(cursor, mesh->normals, bytes);
		cursor += bytes;
	}
	if (mesh->n_uv > 0)
	{
		//Store uv
		bytes = sizeof(float2) * mesh->n_uv;
		memcpy(cursor, mesh->uv_coords, bytes);
		cursor += bytes;
	}
	uint ret = App->file_system->Save(std::string(LIBRARY_MESH_FOLDER + mesh->GetMyGameObject()->GetName() + OUR_EXTENSION).c_str(), data, size);
	RELEASE_ARRAY(data);

	return true;
}


//ComponentMesh* ImportMesh::LoadPrimitive(PRIMITIVE_TYPE type)
//{
//	
//}