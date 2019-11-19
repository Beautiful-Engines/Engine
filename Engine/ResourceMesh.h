#ifndef _ResourceMesh_H_
#define _ResourceMesh_H_

#include "Resource.h"
#include "MathGeoLib\include\Math\float2.h"
#include "MathGeoLib\include\Math\float3.h"

class ResourceMesh : public Resource
{
public:
	ResourceMesh(uint UID = 0);
	~ResourceMesh();
	
	uint id_index = 0u;
	uint n_indexes = 0u;
	uint *indexes = nullptr;

	uint id_vertex = 0u;
	uint n_vertices = 0u;
	float3 *vertices = nullptr;

	uint id_normal = 0u;
	uint n_normals = 0u;
	float3 *normals = nullptr;

	uint id_uv = 0u;
	uint uv_comp = 0u;
	uint n_uv = 0u;
	float2 *uv_coords = nullptr;

	float3 *face_center_point = nullptr;
	float3 *face_normal = nullptr;

	bool is_primitive = false;

	uint id_buffer_texture = 0;
	uint id_buffer_default_texture = 0;
};

#endif // !_ResourceMesh_H_

