#ifndef __ComponentMesh_H__
#define __ComponentMesh_H__

#include "Component.h"
class ComponentMaterial;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* _game_object);
	~ComponentMesh();

	void Update();

	void Draw(ComponentMaterial *component_material);
	void DrawNormals();

public:
	uint id_index = 0u;
	uint n_indexes = 0u;
	uint *indexes = nullptr;

	uint id_vertex = 0u;
	uint n_vertices = 0u;
	float *vertices = nullptr;

	uint id_normal = 0u;
	aiVector3D *normals;

	uint id_uv = 0u;
	uint n_uv = 0u;
	float *uv_coords = nullptr;

	float *face_center_point = nullptr;
	float *face_normal = nullptr;

}; 

#endif // !__ComponentMesh_H__