#include "glew\glew.h"
#include "ResourceMesh.h"

ResourceMesh::ResourceMesh(uint UID) : Resource(Resource::RESOURCE_TYPE::RESOURCE_MESH, UID)
{

}

ResourceMesh::~ResourceMesh()
{
	glDeleteBuffers(1, &id_index);
	glDeleteBuffers(1, &id_normal);
	glDeleteBuffers(1, &id_vertex);
	glDeleteBuffers(1, &id_uv);

	delete[] vertices;
	vertices = nullptr;

	delete[] indexes;
	indexes = nullptr;

	delete[] normals;
	normals = nullptr;

	delete[] uv_coords;
	uv_coords = nullptr;

	delete[] face_center_point;
	face_center_point = nullptr;

	delete[] face_normal;
	face_normal = nullptr;
}