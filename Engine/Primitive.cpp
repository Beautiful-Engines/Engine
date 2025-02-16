#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "ModuleImport.h"
#include "ComponentTransform.h"
#include "Primitive.h"
#include "ResourceMesh.h"

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
		CreateSphere(10, 10);
		break;
	case PrimitiveType::CUBE:
		CreateCube();
		break;
	/*case PrimitiveType::TORUS:
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
		CreateCube();
		break;*/
	default:
		CreateCube();
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
	if (!shape->normals)
	{
		par_shapes_unweld(shape, true);
		par_shapes_compute_normals(shape);
	}
	// Vertices
	n_vertices = shape->npoints;
	vertices = new float3[n_vertices];
	memcpy(vertices, shape->points, sizeof(float3) * n_vertices);

	// Indexes
	n_indexes = shape->ntriangles * 3;
	indexes = new uint[n_indexes];
	memcpy(indexes, shape->triangles, sizeof(uint) * n_indexes);

	// UVs
	if (shape->tcoords) {
		uv_comp = 2;
		n_uv = n_vertices;
		uv_coords = new float2[n_uv * uv_comp];
		memcpy(uv_coords, shape->tcoords, sizeof(float2) * n_uv * uv_comp);
	}

	// Normals
	if (shape->normals)
	{
		// Vertex Normals
		normals = new float3[n_vertices];
		n_normals = n_vertices;
		memcpy(normals, shape->normals, sizeof(float3) * n_normals);

		// Face Normals
		face_center_point = new float3[shape->ntriangles];
		face_normal = new float3[shape->ntriangles];

		/*for (uint i = 0; i < n_indexes; i +=3 )
		{
			uint index = shape->triangles[i];
			vec3 vertex0(vertices[index * 3].x, vertices[index * 3].y, vertices[index * 3].z);

			index = shape->triangles[i + 1];
			vec3 vertex1(vertices[index * 3].x, vertices[index * 3].y, vertices[index * 3].z);

			index = shape->triangles[i + 2];
			vec3 vertex2(vertices[index * 3].x, vertices[index * 3].y, vertices[index * 3].z);

			vec3 v0 = vertex0 - vertex2;
			vec3 v1 = vertex1 - vertex2;
			vec3 n = cross(v0, v1);

			vec3 normalized = normalize(n);

			face_center_point[i].x = (vertex0.x + vertex1.x + vertex2.x) / 3;
			face_center_point[i].y = (vertex0.y + vertex1.y + vertex2.y) / 3;
			face_center_point[i].z = (vertex0.z + vertex1.z + vertex2.z) / 3;

			face_normal[i].x = normalized.x;
			face_normal[i].y = normalized.y;
			face_normal[i].z = normalized.z;
		}*/
	}

	AddToMesh();
	par_shapes_free_mesh(shape);
}

void Primitive::AddToMesh()
{
	ResourceMesh* primitive_mesh = new ResourceMesh(1);
	primitive_mesh->id_index = id_index;
	primitive_mesh->n_indexes = n_indexes;
	primitive_mesh->indexes = indexes;
	primitive_mesh->id_vertex = id_vertex;
	primitive_mesh->n_vertices = n_vertices;
	primitive_mesh->vertices = vertices;
	primitive_mesh->id_normal = id_normal;
	primitive_mesh->n_normals = n_normals;
	primitive_mesh->normals = normals;
	primitive_mesh->id_uv = id_uv;
	primitive_mesh->n_uv = n_uv;
	primitive_mesh->uv_comp = uv_comp;
	primitive_mesh->uv_coords = uv_coords;
	primitive_mesh->face_center_point = face_center_point;
	primitive_mesh->face_normal = face_normal;
	primitive_mesh->is_primitive = true;

	/*App->importer->GLBuffer(primitive_mesh);
	App->importer->DefaultTexture(this);*/
}

// PRIMITIVE FORMS
void Primitive::CreateSphere(const uint& _slices, const uint& _stacks)
{
	std::string name = PutFirstName("Sphere");

	shape = par_shapes_create_parametric_sphere(_slices, _stacks);
	NormalsCalc();
	
	SetName(name);
}

void Primitive::CreateCube()
{
	std::string name = PutFirstName("Cube");

	shape = par_shapes_create_cube();
	NormalsCalc();

	SetName(name);
}

void Primitive::CreateTorus(const uint& slices, const uint& stacks, const float& radius)
{
	std::string name = PutFirstName("Torus");

	shape = par_shapes_create_torus(slices, stacks, radius);
	NormalsCalc();

	SetName(name);
}

void Primitive::CreateOctahedron()
{
	std::string name = PutFirstName("Octahedron");

	shape = par_shapes_create_octahedron();
	NormalsCalc();

	SetName(name);
}

void Primitive::CreateDodecahedron()
{
	std::string name = PutFirstName("Dodecahedron");

	shape = par_shapes_create_dodecahedron();
	NormalsCalc();

	SetName(name);
}

void Primitive::CreateIcosahedron()
{
	std::string name = PutFirstName("Icosahedron");

	shape = par_shapes_create_icosahedron();
	NormalsCalc();

	SetName(name);
}

void Primitive::CreateRock(const uint& _seed, const uint& _subdivisions)
{
	std::string name = PutFirstName("Rock");

	shape = par_shapes_create_rock(_seed, _subdivisions);
	NormalsCalc();

	SetName(name);
}

void Primitive::CreateKleinBottle(const uint& _slices, const uint& _stacks)
{
	std::string name = PutFirstName("Klein_Bottle");

	shape = par_shapes_create_klein_bottle(_slices, _stacks);
	NormalsCalc();

	SetName(name);
}

std::string Primitive::PutFirstName(const std::string& _name)
{
	uint count = 1;
	std::string namecount = _name + std::to_string(count);
	for (uint i = 0; i < App->scene->GetGameObjects().size(); ++i)
	{
		if (App->scene->GetGameObjects()[i] != nullptr && App->scene->GetGameObjects()[i]->GetName() == namecount)
		{
			count++;
			namecount = _name + std::to_string(count);
			i = 0;
		}
	}

	return namecount;
}