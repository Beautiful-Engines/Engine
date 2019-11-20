#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleImport.h"
#include "GameObject.h"
#include "ComponentTexture.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ResourceMesh.h"
#include "ComponentMesh.h"
#include "MathGeoLib\include\Geometry\AABB.h"

#include "glew/glew.h"

ComponentMesh::ComponentMesh(GameObject* _game_object) : Component(_game_object, ComponentType::MESH)
{

}

ComponentMesh::~ComponentMesh()
{

}

void ComponentMesh::Update()
{
	std::vector<Component*> components = my_game_object->GetComponents();
	std::vector<Component*>::iterator iterator_component = components.begin();

	ComponentTexture *component_texture = nullptr;
	for (; iterator_component != components.end(); ++iterator_component)
	{
		if (*iterator_component != nullptr && (*iterator_component)->GetType() == ComponentType::TEXTURE)
		{
			component_texture = (ComponentTexture*)*iterator_component;
		}
	}


	/*if (App->renderer3D->camera->frustum.Intersects(GetMyGameObject()->abb))
	{
		for (uint i = 0; i < App->scene->GetGameObjects().size(); ++i)
		{
			if (App->scene->GetGameObjects()[i]->GetCamera() != nullptr)
			{
				if (App->scene->GetGameObjects()[i]->GetCamera()->frustum_culling == true)
				{
					if (App->scene->GetGameObjects()[i]->GetCamera()->frustum.Intersects(GetMyGameObject()->abb))
					{
						Draw(component_texture);
						break;
					}
				}
				else
				{
					Draw(component_texture);
				}
			}
		}
	}
		Draw(component_texture);
	}*/
	/*if (App->renderer3D->camera->frustum.Intersects(GetMyGameObject()->abb))
	{*/
		Draw(component_texture);
	//}
	if (App->renderer3D->normals || vertex_normals || face_normals)
		DrawNormals();
}

void ComponentMesh::Save(const nlohmann::json::iterator& _iterator)
{
	nlohmann::json json = {
		{"type", type},
		{"vertex_normals",vertex_normals},
		{"face_normals", face_normals},
		{"textures", textures },
		{"debug_bb", debug_bb }
	};

	_iterator.value().push_back(json);
}

void ComponentMesh::Load(const nlohmann::json _json)
{
	type = _json["type"];
	vertex_normals = _json["vertex_normals"];
	face_normals = _json["face_normals"];
	textures = _json["textures"];
	debug_bb = _json["debug_bb"];
}

void ComponentMesh::Draw(ComponentTexture *component_texture)
{
	glPushMatrix();
	glMultMatrixf((float*)&GetMyGameObject()->GetTransform()->transform_matrix.Transposed());

	glEnableClientState(GL_VERTEX_ARRAY);

	glColor3f(1.f, 1.f, 1.f);
	glBindBuffer(GL_ARRAY_BUFFER, resource_mesh->id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resource_mesh->id_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (component_texture != nullptr && textures)
	{
		component_texture->DrawTexture(resource_mesh);
	}

	if (resource_mesh->is_primitive)
		glDrawElements(GL_TRIANGLES, resource_mesh->n_indexes, GL_UNSIGNED_SHORT, NULL);
	else
		glDrawElements(GL_TRIANGLES, resource_mesh->n_indexes, GL_UNSIGNED_INT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();

	if (debug_bb) {
		static float3 corners[8];
		GetMyGameObject()->abb.GetCornerPoints(corners);
		App->renderer3D->DebugDrawCube(corners, { 255, 0, 0, 255 });
		GetMyGameObject()->obb.GetCornerPoints(corners);
		App->renderer3D->DebugDrawCube(corners, { 0, 0, 255, 255 });
	}

}


void ComponentMesh::DrawNormals()
{
	if (resource_mesh->normals != nullptr)
	{
		glColor3f(0.f, 1.f, 0.f);
		glBegin(GL_LINES);
		float lenght = 0.4f;

		if ((App->renderer3D->vertex_normals && App->renderer3D->normals) || vertex_normals)
		{
			int j = 0;
			for (int i = 0; i < resource_mesh->n_vertices; i++) {
				glVertex3f(resource_mesh->vertices[i].x, resource_mesh->vertices[i].y, resource_mesh->vertices[i].z);
				glVertex3f(resource_mesh->vertices[i].x + resource_mesh->normals[j].x  * lenght, resource_mesh->vertices[i].y + resource_mesh->normals[j].y  * lenght, resource_mesh->vertices[i].z + resource_mesh->normals[j].z  * lenght);
				++j;
			}
		}
		if ((App->renderer3D->normals && !App->renderer3D->vertex_normals) || face_normals)
		{
			for (int i = 0; i < resource_mesh->n_indexes; i += 3) {
				glVertex3f(resource_mesh->face_center_point[i].x, resource_mesh->face_center_point[i].y, resource_mesh->face_center_point[i].z);
				glVertex3f(resource_mesh->face_center_point[i].x + resource_mesh->face_normal[i].x * lenght, resource_mesh->face_center_point[i].y + resource_mesh->face_normal[i].y * lenght, resource_mesh->face_center_point[i].z + resource_mesh->face_normal[i].z * lenght);
			}
		}
		glEnd();
	}
}
float3 ComponentMesh::GetMaxPoint()
{
	float3 maxP = resource_mesh->vertices[0];
	for (int i = 0; i < resource_mesh->n_vertices; i++) {
		if (maxP.y < resource_mesh->vertices[i].y)
		{
			maxP = resource_mesh->vertices[i];
		}
	}
	return maxP;
}
AABB ComponentMesh::GetBB()
{
	/*if (debug_bb)
	{
		static float3 corners[8];
		GetMyGameObject()->abb.GetCornerPoints(corners);
		App->renderer3D->DebugDrawCube(corners, { 0, 255, 0, 255 });
		GetMyGameObject()->obb.GetCornerPoints(corners);
		App->renderer3D->DebugDrawCube(corners, { 0, 0, 255, 255 });
		if (App->camera->lines.size() > 0)
			App->renderer3D->DebugDrawLines(App->camera->lines);
	}*/
	AABB bounding_box;
	bounding_box.SetNegativeInfinity();
	bounding_box.Enclose(resource_mesh->vertices, resource_mesh->n_vertices);
	return bounding_box;
}
float3 ComponentMesh::GetMinPoint()
{
	float3 minP = resource_mesh->vertices[0];
	for (int i = 0; i < resource_mesh->n_vertices; i++) {
		if (minP.y > resource_mesh->vertices[i].y)
		{
			minP = resource_mesh->vertices[i];
		}
	}
	return minP;
}

void ComponentMesh::AddResourceMesh(ResourceMesh* _resource_mesh)
{
	resource_mesh = _resource_mesh;
}

ResourceMesh* ComponentMesh::GetResourceMesh()
{
	return resource_mesh;
}

