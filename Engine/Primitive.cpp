#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "Primitive.h"

#include "glew/glew.h"
#include "glmath.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "Par/par_shapes.h"

Primitive::Primitive(PrimitiveType _primitive_type, uint _subdivisions) : GameObject()
{
	
	switch (_primitive_type)
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

void Primitive::Update()
{
	if(shape != nullptr)
		Draw();
	if (App->renderer3D->normals)
		DrawNormals();
}

void Primitive::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glColor3f(1.f, 1.f, 1.f);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, shape->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
}


void Primitive::DrawNormals()
{
	if (normals != nullptr)
	{
		glColor3f(0.f, 1.f, 0.f);
		glBegin(GL_LINES);
		float lenght = 0.4f;

		if (App->renderer3D->vertex_normals)
		{
			int j = 0;
			for (int i = 0; i < n_vertices * 3; i += 3) {
				glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
				glVertex3f(vertices[i] + normals[j].x  * lenght, vertices[i + 1] + normals[j].y  * lenght, vertices[i + 2] + normals[j].z  * lenght);
				++j;
			}
		}
		else {

			for (int i = 0; i < n_indexes; i += 3) {
				glVertex3f(face_center_point[i], face_center_point[i + 1], face_center_point[i + 2]);
				glVertex3f(face_center_point[i] + face_normal[i] * lenght, face_center_point[i + 1] + face_normal[i + 1] * lenght, face_center_point[i + 2] + face_normal[i + 2] * lenght);
			}
		}

		glEnd();
	}
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

void Primitive::NormalsCalc()
{
	n_vertices = shape->npoints;
	vertices = new float[n_vertices * 3];
	memcpy(vertices, shape->points, sizeof(float) * n_vertices * 3);

	n_indexes = shape->ntriangles * 3;
	indexes = new uint[n_indexes * 3];
	memcpy(indexes, shape->triangles, sizeof(uint) * n_indexes);

	if (shape->tcoords != nullptr) {
		uv_coords = new float[n_vertices * 3];
		memcpy(uv_coords, shape->tcoords, sizeof(float) * n_vertices * 3);
	}

	// Normals
	if (shape->normals != nullptr)
	{
		normals = new aiVector3D[n_vertices * 3];
		memcpy(normals, shape->normals, sizeof(aiVector3D) * n_vertices * 3);

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

	switch (primitive_type)
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

// PRIMITIVE FORMS
void Primitive::CreateSphere(const uint& _subdivisions)
{
	std::string name = "Sphere";
	std::vector<GameObject*> game_object = App->scene->GetGameObjects();
	uint cont = 1;
	for (uint i = 0; i < game_object.size(); ++i)
	{
		name = name + std::to_string(cont);
		if (game_object[i] != nullptr && game_object[i]->GetName() == name)
		{
			cont++;
			i = 0;
		}
	}

	shape = par_shapes_create_subdivided_sphere(_subdivisions);
	NormalsCalc();
	GLBuffers();
	
	SetName(name);
}

