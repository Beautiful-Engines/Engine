#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ComponentMesh.h"

#include "glew/glew.h"

ComponentMesh::ComponentMesh(GameObject* _game_object) : Component(_game_object, ComponentType::MESH)
{
	_game_object->AddComponent(this);
}

ComponentMesh::~ComponentMesh()
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

void ComponentMesh::Update(float dt)
{
	Draw();
	if (App->renderer3D->normals)
		DrawNormals();
}

void ComponentMesh::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glColor3f(1.f, 1.f, 1.f);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, n_indexes, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
}


void ComponentMesh::DrawNormals()
{
	if (normals != nullptr)
	{
		glColor3f(0.f, 1.f, 0.f);
		glBegin(GL_LINES);

		if (App->renderer3D->vertex_normals)
		{
			int j = 0;
			for (int i = 0; i < n_vertices * 3; i += 3) {
				glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
				glVertex3f(vertices[i] + normals[j].x, vertices[i + 1] + normals[j].y, vertices[i + 2] + normals[j].z);
				++j;
			}
		}
		else {

			for (int i = 0; i < n_indexes; i += 3) {
				glVertex3f(face_center_point[i], face_center_point[i + 1], face_center_point[i + 2]);
				glVertex3f(face_center_point[i] + face_normal[i], face_center_point[i + 1] + face_normal[i + 1], face_center_point[i + 2] + face_normal[i + 2]);
			}
		}

		glEnd();
	}
}
