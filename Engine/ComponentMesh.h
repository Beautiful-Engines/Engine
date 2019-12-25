#ifndef __ComponentMesh_H__
#define __ComponentMesh_H__


#include "Component.h"
#include "MathGeoLib\include\Math\float2.h"
#include "MathGeoLib\include\Math\float3.h"

class ComponentTexture;
class ResourceMesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* _game_object);
	~ComponentMesh();

	void Update(float dt);

	void Save(const nlohmann::json::iterator& _iterator);
	void Load(const nlohmann::json _json);

	float3 GetMinPoint();
	AABB GetBB();
	float3 GetMaxPoint();

	void AddResourceMesh(ResourceMesh* _resource_mesh);
	ResourceMesh* GetResourceMesh();
	
	void Draw(ComponentTexture *component_texture);
	void DrawNormals();

public:

	bool vertex_normals = false;
	bool face_normals = false;
	bool textures = true;
	
	bool debug_bb = true;
	bool draw = true;

private:
	ResourceMesh* resource_mesh = nullptr;

}; 

#endif // !__ComponentMesh_H__