#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "ModuleImport.h"
#include "ComponentTransform.h"
#include "Primitive.h"

#include "glew/glew.h"
#include "glmath.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "Par/par_shapes.h"

Primitive::Primitive(PrimitiveType _primitive_type) : GameObject()
{
	primitive_type = _primitive_type;

	switch (_primitive_type)
	{
	case PrimitiveType::SPHERE:
		CreateSphere(5);
		break;
	case PrimitiveType::CUBE:
		CreateCube();
		break;
	case PrimitiveType::TORUS:
		CreateTorus(10, 20, 0.5f);
		break;
	case PrimitiveType::OCTAHEDRON:
		CreateOctahedron();
		break;
	case PrimitiveType::DODECAHEDRON:
		CreateDodecahedron();
		break;
	case PrimitiveType::ICOSAHEDRON:
		CreateIcosahedron();
		break;
	case PrimitiveType::ROCK:
		CreateRock(5, 5);
		break;
	case PrimitiveType::KLEIN_BOTTLE:
		CreateKleinBottle(20, 20);
		break;
	case PrimitiveType::OTHER:
		break;
	default:
		break;
	}

	SetParent(App->scene->GetGameObjects()[0]);
	App->scene->AddGameObject(this);
	
}

Primitive::~Primitive()
{
	par_shapes_free_mesh(shape);
}

void Primitive::NormalsCalc()
{
	par_shapes_unweld(shape, true);
	par_shapes_compute_normals(shape);

	n_vertices = shape->npoints;
	vertices = new float[n_vertices * 3];
	memcpy(vertices, shape->points, sizeof(float) * n_vertices * 3);

	n_indexes = shape->ntriangles * 3;
	indexes = new uint[n_indexes * 3];
	memcpy(indexes, shape->triangles, sizeof(PAR_SHAPES_T) * n_indexes);

	if (shape->tcoords) {
		uv_coords = new float[n_vertices * 2];
		memcpy(uv_coords, shape->tcoords, sizeof(float) * n_vertices * 2);
	}

	// Normals
	if (primitive_type != PrimitiveType::CUBE && primitive_type != PrimitiveType::OCTAHEDRON && primitive_type != PrimitiveType::DODECAHEDRON && primitive_type != PrimitiveType::ICOSAHEDRON)
	{
		if (shape->normals != nullptr)
		{
			normals = new float[n_vertices * 3];
			memcpy(normals, shape->normals, sizeof(float) * n_vertices * 3);

			face_center_point = new float[shape->ntriangles * 3];
			face_normal = new float[shape->ntriangles * 3];

			// TODO
			/*for (uint i = 0; i < n_indexes; i += 3)
			{
				uint index = indexes[i];
				vec3 vertex0(vertices[index], vertices[index + 1], vertices[index + 2]);

				index = indexes[i + 1];
				vec3 vertex1(vertices[index], vertices[index + 1], vertices[index+ 2]);

				index = indexes[i + 2];
				vec3 vertex2(vertices[index], vertices[index+ 1], vertices[index + 2]);

				vec3 v0 = vertex0 - vertex2;
				vec3 v1 = vertex1 - vertex2;
				vec3 n = cross(v0, v1);

				vec3 normalized = normalize(n);

				face_center_point[i] = (vertex0.x + vertex1.x + vertex2.x) / 3;
				face_center_point[i + 1] = (vertex0.y + vertex1.y + vertex2.y) / 3;
				face_center_point[i + 2] = (vertex0.z + vertex1.z + vertex2.z) / 3;

				face_normal[i] = normalized.x;
				face_normal[i + 1] = normalized.y;
				face_normal[i + 2] = normalized.z;
			}*/
		}
	}
	

	AddToMesh();
	par_shapes_free_mesh(shape);
}

void Primitive::AddToMesh()
{
	ComponentMesh* primitive_mesh = new ComponentMesh(this);
	primitive_mesh->id_index = id_index;
	primitive_mesh->n_indexes = n_indexes;
	primitive_mesh->indexes = indexes;
	primitive_mesh->id_vertex = id_vertex;
	primitive_mesh->n_vertices = n_vertices;
	primitive_mesh->vertices = vertices;
	primitive_mesh->id_normal = id_normal;
	primitive_mesh->normals = (aiVector3D*)normals;
	primitive_mesh->id_uv = id_uv;
	primitive_mesh->n_uv = n_uv;
	primitive_mesh->uv_coords = uv_coords;
	primitive_mesh->face_center_point = face_center_point;
	primitive_mesh->face_normal = face_normal;
	primitive_mesh->is_primitive = true;

	App->importer->GLBuffer(primitive_mesh);
	App->importer->DefaultTexture(this);
}

void Primitive::RestartBuffers()
{
	glDeleteBuffers(1, &id_vertex);

	if (shape->normals != nullptr) {
		glDeleteBuffers(1, &id_normal);
		delete[] normals;
	}
	if (shape->tcoords != nullptr) {
		glDeleteBuffers(1, &id_uv);
		delete[] uv_coords;
	}
	
	delete[] vertices;
	delete[] indexes;

	NormalsCalc();
	/*GLBuffers();*/
}

void Primitive::SetPosition(const float& _x, const float& _y, const float& _z)
{
	par_shapes_translate(shape, _x, _y, _z);
}

void Primitive::SetSubdivisions(const int& _subdivisions)
{
	par_shapes_free_mesh(shape);
	subdivisions = _subdivisions;

	switch (primitive_type)
	{
	case PrimitiveType::SPHERE:
		CreateSphere(_subdivisions);
		break;
	case PrimitiveType::ROCK:
		CreateRock(5, _subdivisions);
		break;
	default:
		break;
	}

	RestartBuffers();
}

// PRIMITIVE FORMS
void Primitive::CreateSphere(const uint& _subdivisions)
{
	std::string name = "Sphere";
	std::string namecount = name + "1";
	namecount = PutFirstName(name, namecount);

	shape = par_shapes_create_subdivided_sphere(_subdivisions);
	NormalsCalc();
	
	SetName(namecount);
}

void Primitive::CreateCube()
{
	std::string name = "Cube";
	std::string namecount = name + "1";
	namecount = PutFirstName(name, namecount);

	shape = par_shapes_create_cube();
	NormalsCalc();

	SetName(namecount);
}

void Primitive::CreateTorus(const uint& slices, const uint& stacks, const float& radius)
{
	std::string name = "Torus";
	std::string namecount = name + "1";
	namecount = PutFirstName(name, namecount);

	shape = par_shapes_create_torus(slices, stacks, radius);
	NormalsCalc();

	SetName(namecount);
}

void Primitive::CreateOctahedron()
{
	std::string name = "Octahedron";
	std::string namecount = name + "1";
	namecount = PutFirstName(name, namecount);

	shape = par_shapes_create_octahedron();
	NormalsCalc();

	SetName(namecount);
}

void Primitive::CreateDodecahedron()
{
	std::string name = "Dodecahedron";
	std::string namecount = name + "1";
	namecount = PutFirstName(name, namecount);

	shape = par_shapes_create_dodecahedron();
	NormalsCalc();

	SetName(namecount);
}

void Primitive::CreateIcosahedron()
{
	std::string name = "Icosahedron";
	std::string namecount = name + "1";
	namecount = PutFirstName(name, namecount);

	shape = par_shapes_create_icosahedron();
	NormalsCalc();

	SetName(namecount);
}

void Primitive::CreateRock(const uint& _seed, const uint& _subdivisions)
{
	std::string name = "Rock";
	std::string namecount = name + "1";
	namecount = PutFirstName(name, namecount);

	shape = par_shapes_create_rock(_seed, _subdivisions);
	NormalsCalc();

	SetName(namecount);
}

void Primitive::CreateKleinBottle(const uint& _slices, const uint& _stacks)
{
	std::string name = "Klein_Bottle";
	std::string namecount = name + "1";
	namecount = PutFirstName(name, namecount);

	shape = par_shapes_create_klein_bottle(_slices, _stacks);
	NormalsCalc();

	SetName(namecount);
}

std::string Primitive::PutFirstName(const std::string& _name, std::string _namecount)
{
	std::vector<GameObject*> game_object = App->scene->GetGameObjects();
	uint count = 1;
	for (uint i = 0; i < game_object.size(); ++i)
	{
		if (game_object[i] != nullptr && game_object[i]->GetName() == _namecount)
		{
			count++;
			_namecount = _name + std::to_string(count);
			i = 0;
		}
	}

	return _namecount;
}