#ifndef __Primitive_H__
#define __Primitive_H__

#include "GameObject.h"
#include "Assimp/include/vector3.h"

enum class PrimitiveType
{
	SPHERE,
	CUBE,
	TORUS,
	OCTAHEDRON,
	DODECAHEDRON,
	ICOSAHEDRON,
	ROCK,
	KLEIN_BOTTLE,

	OTHER
};

typedef struct par_shapes_mesh_s par_shapes_mesh;

class Primitive : public GameObject
{
public:
	Primitive(PrimitiveType _primitive_type);
	virtual ~Primitive();

	void Update();

	void Draw();
	void DrawNormals();

	PrimitiveType primitive_type = PrimitiveType::OTHER;

	void SetPosition(const float& _x, const float& _y, const float& _z);

	void SetSubdivisions(const int& _subdivisions);

private:

	void GLBuffers();
	void NormalsCalc();
	void AddToMesh();
	void RestartBuffers();
	void CreateSphere(const uint& _subdivisions);
	void CreateCube();
	void CreateTorus(const uint& _slices, const uint& _stacks, const float& _radius);
	void CreateOctahedron();
	void CreateDodecahedron();
	void CreateIcosahedron();
	void CreateRock(const uint& _seed, const uint& _subdivisions);
	void CreateKleinBottle(const uint& _slices, const uint& _stacks);
	std::string PutFirstName(const std::string& _name, std::string _namecount);


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

	par_shapes_mesh* shape = nullptr;
	int subdivisions = 5;

	aiVector3D position = { 0,0,0 };

	uint material_index = 0;
	int id_texture = -1;

};

#endif // !__Primitive_H__