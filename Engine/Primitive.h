#ifndef __Primitive_H__
#define __Primitive_H__

#include "GameObject.h"
#include "Assimp/include/vector3.h"

enum class PrimitiveType
{
	SPHERE,
	OTHER
};

typedef struct par_shapes_mesh_s par_shapes_mesh;

class Primitive : public GameObject
{
public:
	Primitive(PrimitiveType _type, uint _subdivisions);
	virtual ~Primitive();

	PrimitiveType type = PrimitiveType::OTHER;

	void SetPosition(const float& _x, const float& _y, const float& _z);

	void SetSubdivisions(const int& _subdivisions);

private:

	void GLBuffers();
	void MemCpy();
	void RestartBuffers();
	void CreateSphere(const uint& _subdivisions);

private:
	uint id_index = 0u;  // index in VRAM
	uint n_indexes = 0u;
	uint *indexes = nullptr;

	uint id_vertex = 0u;  // unique vertex in VRAM
	uint n_vertices = 0u;
	float *vertices = nullptr;

	uint id_normal = 0u;
	aiVector3D *normals;

	uint id_uv = 0u;
	float *uv_coords = nullptr;

	float *face_center_point = nullptr;
	float *face_normal = nullptr;

	par_shapes_mesh* shape = nullptr;
	int subdivisions = 5;

	aiVector3D position = { 0,0,0 };

	uint material_index = 0;
	int id_texture = -1;

};

#endif // !__Primitive_H__