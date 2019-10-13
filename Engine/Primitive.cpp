#include "Globals.h"
#include "Primitive.h"

#include "glew/glew.h"
#include "glmath.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "Par/par_shapes.h"

Primitive::Primitive(PrimitiveType _type, uint _subdivisions) : GameObject()
{
	switch (type)
	{
	case PrimitiveType::SPHERE:
		CreateSphere(_subdivisions);
		break;
	case PrimitiveType::OTHER:
		break;
	default:
		break;
	}
}

Primitive::~Primitive()
{
	par_shapes_free_mesh(shape);
}

void Primitive::GLBuffers()
{
	// buffer points
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * shape->npoints * 3, shape->points, GL_STATIC_DRAW);

	// buffer index
	glGenBuffers(1, &id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * shape->ntriangles * 3, shape->triangles, GL_STATIC_DRAW);

	// buffer uv
	glGenBuffers(1, &id_uv);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_uv);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * n_vertices * 3, uv_coords, GL_STATIC_DRAW);

	// normals
	glGenBuffers(1, &id_normal);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_normal);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) *n_vertices * 3, &normals[0], GL_STATIC_DRAW);
}

void Primitive::MemCpy()
{
	n_vertices = shape->npoints;
	n_indexes = shape->ntriangles * 3;

	vertices = new float[n_vertices * 3];
	indexes = new uint[n_indexes * 3];

	memcpy(vertices, shape->points, sizeof(float) * n_vertices * 3);
	memcpy(indexes, shape->triangles, sizeof(PAR_SHAPES_T) * n_indexes);

	if (shape->tcoords != nullptr) {
		uv_coords = new float[n_vertices * 3];
		memcpy(uv_coords, shape->tcoords, sizeof(float) * n_vertices * 3);
	}

	// Normals
	if (shape->normals != nullptr)
	{
		normals = new aiVector3D[n_vertices];
		memcpy(normals, shape->normals, sizeof(aiVector3D) * n_vertices);

		face_center_point = new float[shape->ntriangles * 3];
		face_normal = new float[shape->ntriangles * 3];

		for (uint i = 0; i < n_indexes; i += 3)
		{
			uint index = indexes[i];
			vec3 x0(vertices[index * 3], vertices[index * 3 + 1], vertices[index * 3 + 2]);
			index = indexes[i + 1];
			vec3 x1(vertices[index * 3], vertices[index * 3 + 1], vertices[index * 3 + 2]);
			index = indexes[i + 2];
			vec3 x2(vertices[index * 3], vertices[index * 3 + 1], vertices[index * 3 + 2]);

			vec3 v0 = x0 - x2;
			vec3 v1 = x1 - x2;
			vec3 n = cross(v0, v1);

			vec3 normalized = normalize(n);

			face_center_point[i] = (x0.x + x1.x + x2.x) / 3;
			face_center_point[i + 1] = (x0.y + x1.y + x2.y) / 3;
			face_center_point[i + 2] = (x0.z + x1.z + x2.z) / 3;

			face_normal[i] = normalized.x;
			face_normal[i + 1] = normalized.y;
			face_normal[i + 2] = normalized.z;
		}
	}

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

	MemCpy();
	GLBuffers();
}

void Primitive::SetPosition(const float& _x, const float& _y, const float& _z)
{
	par_shapes_translate(shape, _x, _y, _z);
}

void Primitive::SetSubdivisions(const int& _subdivisions)
{
	par_shapes_free_mesh(shape);
	subdivisions = _subdivisions;

	switch (type)
	{
	case PrimitiveType::SPHERE:
		par_shapes_create_subdivided_sphere(_subdivisions);
		break;
	case PrimitiveType::OTHER:
		break;
	default:
		break;
	}

	RestartBuffers();
}

void Primitive::CreateSphere(const uint& _subdivisions)
{
	shape = par_shapes_create_subdivided_sphere(_subdivisions);
	MemCpy();
	GLBuffers();
}