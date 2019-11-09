#ifndef __ComponentMesh_H__
#define __ComponentMesh_H__

#include "Component.h"
#include "MathGeoLib\include\Math\float2.h"
#include "MathGeoLib\include\Math\float3.h"
class ComponentMaterial;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* _game_object);
	~ComponentMesh();

	void Update();

	float3 GetMinPoint();
	AABB GetBB();
	float3 GetMaxPoint();

	void Draw(ComponentMaterial *component_material);
	void DrawNormals();

public:
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

	bool checkered = false;
	uint id_texture = 0;
	uint id_default_texture = 0;

	bool is_primitive = false;

	bool vertex_normals = false;
	bool face_normals = false;
	bool textures = true;
	

	bool debug_bb = true;
}; 

#endif // !__ComponentMesh_H__