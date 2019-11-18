#include "Application.h"
#include "ModuleFileSystem.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "ResourceMesh.h"
#include "GameObject.h"

#include "glew/glew.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "ImportMesh.h"

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

void ImportMesh::Import(const aiScene* scene, const aiMesh* ai_mesh, ResourceMesh* _resource_mesh)
{

	_resource_mesh->n_vertices = ai_mesh->mNumVertices;
	_resource_mesh->vertices = new float3[_resource_mesh->n_vertices];
	memcpy(_resource_mesh->vertices, ai_mesh->mVertices, sizeof(float3) * _resource_mesh->n_vertices);

	// Faces
	if (ai_mesh->HasFaces())
	{
		_resource_mesh->n_indexes = ai_mesh->mNumFaces * 3;
		_resource_mesh->indexes = new uint[_resource_mesh->n_indexes];

		for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
		{
			if (ai_mesh->mFaces[j].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&_resource_mesh->indexes[j * 3], ai_mesh->mFaces[j].mIndices, sizeof(uint) * 3);
			}
		}
		LOG("New resource mesh %s, with %i vertices and %i faces has been added", _resource_mesh->GetName(), _resource_mesh->n_vertices, _resource_mesh->n_indexes / 3);
	}
	// Normals
	if (ai_mesh->HasNormals())
	{
		_resource_mesh->normals = new float3[_resource_mesh->n_vertices];
		_resource_mesh->n_normals = _resource_mesh->n_vertices;
		memcpy(_resource_mesh->normals, ai_mesh->mNormals, sizeof(float3) * _resource_mesh->n_normals);

		_resource_mesh->face_center_point = new float3[ai_mesh->mNumFaces];
		_resource_mesh->face_normal = new float3[ai_mesh->mNumFaces];

		/*for (uint i = 0; i < mymesh->n_indexes; i += 3)
		{
			uint index = mymesh->indexes[i];
			vec3 vertex0(mymesh->vertices[index *3 + 1].x, mymesh->vertices[index * 3 + 2].y, mymesh->vertices[index * 3 + 3].z);

			index = mymesh->indexes[i + 1];
			vec3 vertex1(mymesh->vertices[index * 3 + 1].x, mymesh->vertices[index * 3 + 2].y, mymesh->vertices[index * 3 + 3].z);

			index = mymesh->indexes[i + 2];
			vec3 vertex2(mymesh->vertices[index * 3 + 1].x, mymesh->vertices[index * 3 + 2].y, mymesh->vertices[index * 3 + 3].z);

			vec3 v0 = vertex0 - vertex2;
			vec3 v1 = vertex1 - vertex2;
			vec3 n = cross(v0, v1);

			vec3 normalized = normalize(n);

			mymesh->face_center_point[i].x = (vertex0.x + vertex1.x + vertex2.x) / 3;
			mymesh->face_center_point[i].y = (vertex0.y + vertex1.y + vertex2.y) / 3;
			mymesh->face_center_point[i].z = (vertex0.z + vertex1.z + vertex2.z) / 3;

			mymesh->face_normal[i].x = normalized.x;
			mymesh->face_normal[i].y = normalized.y;
			mymesh->face_normal[i].z = normalized.z;
		}*/
	}

	// UVs
	if (ai_mesh->HasTextureCoords(0))
	{
		_resource_mesh->uv_comp = ai_mesh->mNumUVComponents[0];
		_resource_mesh->n_uv = _resource_mesh->n_vertices;
		_resource_mesh->uv_coords = new float2[_resource_mesh->n_uv];

		for (uint i = 0; i < _resource_mesh->n_uv; i++)
		{
			memcpy(&_resource_mesh->uv_coords[i], &ai_mesh->mTextureCoords[0][i], sizeof(float2));
		}
	}

	CreateOurMesh(_resource_mesh);
}

bool ImportMesh::CreateOurMesh(ResourceMesh* mesh)
{
	// amount of indexes / vertices / normals / texture_coords
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
	std::string file = LIBRARY_MESH_FOLDER + std::to_string(mesh->GetId()) + OUR_MESH_EXTENSION;
	uint ret = App->file_system->Save(file.c_str(), data, size);
	RELEASE_ARRAY(data);

	return ret;
}

void ImportMesh::LoadMeshFromResource(ResourceMesh* _resource_mesh)
{
	char* buffer;
	uint size = App->file_system->Load(_resource_mesh->GetFile(), &buffer);

	char* cursor = buffer;
	// amount of indexes / vertices / normals / texture_coords
	uint ranges[4];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	_resource_mesh->n_indexes = ranges[0];
	_resource_mesh->n_vertices = ranges[1];
	_resource_mesh->n_normals = ranges[2];
	_resource_mesh->n_uv = ranges[3];
	cursor += bytes;

	// Load indexes
	bytes = sizeof(uint) * _resource_mesh->n_indexes;
	_resource_mesh->indexes = new uint[_resource_mesh->n_indexes];
	memcpy(_resource_mesh->indexes, cursor, bytes);
	cursor += bytes;

	// Load vertices
	bytes = sizeof(float3) * _resource_mesh->n_vertices;
	_resource_mesh->vertices = new float3[_resource_mesh->n_vertices];
	memcpy(_resource_mesh->vertices, cursor, bytes);
	cursor += bytes;

	if (_resource_mesh->n_normals)
	{
		// Load normal
		bytes = sizeof(float3) * _resource_mesh->n_normals;
		_resource_mesh->normals = new float3[_resource_mesh->n_normals];
		memcpy(_resource_mesh->normals, cursor, bytes);
		cursor += bytes;
	}
	if (_resource_mesh->n_uv)
	{
		// Load uv
		bytes = sizeof(float2) * _resource_mesh->n_uv;
		_resource_mesh->uv_coords = new float2[_resource_mesh->n_uv];
		memcpy(_resource_mesh->uv_coords, cursor, bytes);
		cursor += bytes;
	}
	GLBuffer(_resource_mesh);

	RELEASE_ARRAY(buffer);

}

void ImportMesh::GLBuffer(ResourceMesh *mesh)
{
	// VERTICES
	glGenBuffers(1, &mesh->id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * mesh->n_vertices, mesh->vertices, GL_STATIC_DRAW);

	// INDEXES
	glGenBuffers(1, &mesh->id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->n_indexes, mesh->indexes, GL_STATIC_DRAW);

	//NORMALS
	if (mesh->normals != nullptr) {
		glGenBuffers(1, &mesh->id_normal);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normal);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * mesh->n_normals, mesh->normals, GL_STATIC_DRAW);
	}

	// UV
	glGenBuffers(1, &mesh->id_uv);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_uv);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float2) * mesh->n_uv, mesh->uv_coords, GL_STATIC_DRAW);

}