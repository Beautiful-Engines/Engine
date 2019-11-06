#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleImport.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "ModuleRenderer3D.h"
#include "ComponentMesh.h"
#include "MathGeoLib\include\Geometry\AABB.h"

#include "glew/glew.h"

ComponentMesh::ComponentMesh(GameObject* _game_object) : Component(_game_object, ComponentType::MESH)
{
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

void ComponentMesh::Update()
{
	std::vector<Component*> components = my_game_object->GetComponents();
	std::vector<Component*>::iterator iterator_component = components.begin();

	ComponentMaterial *component_material = nullptr;
	for (; iterator_component != components.end(); ++iterator_component) 
	{
		if (*iterator_component != nullptr && (*iterator_component)->GetType() == ComponentType::MATERIAL)
		{
			component_material = (ComponentMaterial*)*iterator_component;
			if (!checkered && component_material->id_texture == this->id_texture)
				break;
			else if (checkered && component_material->id_texture == this->id_default_texture)
				break;
			else
				component_material = nullptr;
		}
	}

	Draw(component_material);
	if (App->renderer3D->normals || vertex_normals || face_normals)
		DrawNormals();

	AABB o = GetMyGameObject()->abb = GetBB();
	GetMyGameObject()->obb = GetBB();
	GetMyGameObject()->UpdateBB();

	if (debug_bb)
	{
		static float3 corners[8];
		GetMyGameObject()->abb.GetCornerPoints(corners);
		App->renderer3D->DebugDrawCube(corners, { 0, 255, 0, 255 });
		GetMyGameObject()->obb.GetCornerPoints(corners);
		App->renderer3D->DebugDrawCube(corners, { 0, 255, 0, 255 });
	}

}

void ComponentMesh::Draw(ComponentMaterial *component_material)
{
	glPushMatrix();
	glMultMatrixf((float*)&GetMyGameObject()->GetTransform()->transform_matrix.Transposed());

	glEnableClientState(GL_VERTEX_ARRAY);

	glColor3f(1.f, 1.f, 1.f);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (component_material != nullptr && textures)
	{
		component_material->DrawTexture(this);
	}

	if(is_primitive)
		glDrawElements(GL_TRIANGLES, n_indexes, GL_UNSIGNED_SHORT, NULL);
	else
		glDrawElements(GL_TRIANGLES, n_indexes, GL_UNSIGNED_INT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}


void ComponentMesh::DrawNormals()
{
	if (normals != nullptr)
	{
		glColor3f(0.f, 1.f, 0.f);
		glBegin(GL_LINES);
		float lenght = 0.4f;

		if ((App->renderer3D->vertex_normals && App->renderer3D->normals) || vertex_normals)
		{
			int j = 0;
			for (int i = 0; i < n_vertices * 3; i += 3) {
				glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
				glVertex3f(vertices[i] + normals[j].x  * lenght, vertices[i + 1] + normals[j].y  * lenght, vertices[i + 2] + normals[j].z  * lenght);
				++j;
			}
		}
		if((App->renderer3D->normals && !App->renderer3D->vertex_normals) || face_normals)
		{
			for (int i = 0; i < n_indexes; i += 3) {
				glVertex3f(face_center_point[i], face_center_point[i + 1], face_center_point[i + 2]);
				glVertex3f(face_center_point[i] + face_normal[i] * lenght, face_center_point[i + 1] + face_normal[i + 1] * lenght, face_center_point[i + 2] + face_normal[i + 2] * lenght);
			}
		}
		glEnd();
	}
}
float3 ComponentMesh::GetMaxPoint()
{
	float3 maxP = { vertices[0],vertices[1],vertices[2] };
	for (int i = 0; i < n_vertices; i += 3) {
			if (maxP.y < vertices[i + 1])
			{
				maxP = { vertices[i],vertices[i + 1],vertices[i + 2] };
			}
	}
	return maxP;
}
AABB ComponentMesh::GetBB()
{
	AABB bounding_box;
	bounding_box.SetNegativeInfinity();
	bounding_box.Enclose(&(float3)vertices, n_vertices);
	return bounding_box;
}
float3 ComponentMesh::GetMinPoint()
{
	float3 minP = { vertices[0],vertices[1],vertices[2] };
	for (int i = 0; i < n_vertices; i += 3) {
		if (minP.y > vertices[i + 1])
		{
			minP = { vertices[i],vertices[i + 1],vertices[i + 2] };
		}
	}
	return minP;
}
