#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleImport.h"
#include "GameObject.h"
#include "ComponentTexture.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleResource.h"
#include "ImportMesh.h"
#include "ResourceMesh.h"
#include "ResourceModel.h"
#include "ResourceBone.h"
#include "ResourceAnimation.h"
#include "ComponentMesh.h"
#include "MathGeoLib\include\Geometry\AABB.h"

#include "glew/glew.h"

ComponentMesh::ComponentMesh(GameObject* _game_object) : Component(_game_object, ComponentType::MESH)
{

}

ComponentMesh::~ComponentMesh()
{

}

void ComponentMesh::Update(float dt)
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
	if (draw)
	{
		Skining();
		Draw(component_texture);
	}
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
		{"debug_bb", debug_bb },
		{"resource_mesh", resource_mesh->GetId() },
		{"resource_mesh_file", resource_mesh->GetFile() },
		{"resource_mesh_default_texture", resource_mesh->id_buffer_default_texture },
		{"resource_mesh_texture", resource_mesh->id_buffer_texture }
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
	resource_mesh = (ResourceMesh*) App->resource->Get(_json["resource_mesh"]);
	if (resource_mesh == nullptr)
	{
		resource_mesh = (ResourceMesh*)App->resource->CreateResource(OUR_MESH_EXTENSION, _json["resource_mesh"]);
		resource_mesh->SetFile(_json["resource_mesh_file"]);
		resource_mesh->id_buffer_default_texture = App->resource->GetId("default_texture");
		resource_mesh->id_buffer_texture = _json["resource_mesh_texture"];
		App->importer->import_mesh->LoadMeshFromResource((ResourceMesh*)resource_mesh);
		
	}
}

void ComponentMesh::Draw(ComponentTexture *component_texture)
{
	glPushMatrix();
	glMultMatrixf((float*)&GetMyGameObject()->GetTransform()->transform_matrix.Transposed());

	glEnableClientState(GL_VERTEX_ARRAY);

	glColor3f(1.f, 1.f, 1.f);
	
	if (deformable_mesh == nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, resource_mesh->id_vertex);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resource_mesh->id_index);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, resource_mesh->id_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * resource_mesh->n_vertices, deformable_mesh->vertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resource_mesh->id_index);
	}
		
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
		App->renderer3D->DebugDrawCube(corners, { 0, 255, 0, 255 });
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
	_resource_mesh->SetCantities(1);
}

ResourceMesh* ComponentMesh::GetResourceMesh()
{
	return resource_mesh;
}

void ComponentMesh::Skining()
{
	// searching the animation
	GameObject* go = this->GetMyGameObject();
	
	while (go->GetAnimation() == nullptr && go->GetName() != "root")
	{
		go = go->GetParent();
	}

	if (go->GetAnimation() != nullptr && resource_mesh != nullptr && go->GetAnimation()->resource_animation != nullptr && go->GetAnimation()->resource_animation->num_channels > 1)
	{
		deformable_mesh = new ResourceMesh();
		deformable_mesh->vertices = new float3[resource_mesh->n_vertices];
		deformable_mesh->normals = new float3[resource_mesh->n_normals];
		memset(deformable_mesh->vertices, 0, sizeof(float3) * resource_mesh->n_vertices);
		memset(deformable_mesh->normals, 0, sizeof(float3) * resource_mesh->n_normals);

		for (int i = 0; i < go->GetAnimation()->resource_animation->num_channels; i++)
		{
			GameObject* go_node = App->scene->GetGameObjectByName(go->GetAnimation()->resource_animation->nodes[i].name_node);
			
			if (go_node->GetBone() != nullptr)
			{
				ResourceBone* resource_bone = go_node->GetBone()->resource_bone;
				if (resource_bone->id_mesh == this->GetResourceMesh()->GetId())
				{
					float4x4 transform = go_node->GetTransform()->GetTransformMatrix();
					transform = GetMyGameObject()->GetTransform()->GetTransformMatrix().Inverted() * transform;
					transform = transform * resource_bone->offset;

					for (int j = 0; j < resource_bone->num_weights; j++)
					{
						uint vertex_index = resource_bone->weights[j].vertex_id;
						if (vertex_index >= resource_mesh->n_vertices)
							continue;

						// transforming mesh position
						float3 matrix_position = transform.TransformPos(resource_mesh->vertices[vertex_index]);

						deformable_mesh->vertices[vertex_index].x += matrix_position.x * resource_bone->weights[j].weight;
						deformable_mesh->vertices[vertex_index].y += matrix_position.y * resource_bone->weights[j].weight;
						deformable_mesh->vertices[vertex_index].z += matrix_position.z * resource_bone->weights[j].weight;

						// transforming normals
						if (resource_mesh->n_normals > 0)
						{
							matrix_position = transform.TransformPos(resource_mesh->normals[vertex_index]);
							deformable_mesh->normals[vertex_index].x += matrix_position.x * resource_bone->weights[j].weight;
							deformable_mesh->normals[vertex_index].y += matrix_position.y * resource_bone->weights[j].weight;
							deformable_mesh->normals[vertex_index].z += matrix_position.z * resource_bone->weights[j].weight;
						}
					}
				}
			}
		}
	}
}

