#include "ComponentTransform.h"
#include "GameObject.h"


ComponentTransform::ComponentTransform(GameObject* _game_object) : Component(_game_object, ComponentType::TRANSFORM)
{
	position = { 0.0f, 0.0f, 0.0f };
	rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	euler = { 0.0f, 0.0f, 0.0f };
	scale = { 1.0f, 1.0f, 1.0f };

	local_position = { 0.0f, 0.0f, 0.0f };
	local_rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	local_euler = { 0.0f, 0.0f, 0.0f };
	local_scale = { 1.0f, 1.0f, 1.0f };

	local_transform_matrix = float4x4::FromTRS(local_position, local_rotation, local_scale);
	transform_matrix = float4x4::FromTRS(position, rotation, scale);
}


ComponentTransform::~ComponentTransform()
{
}

// Save
void ComponentTransform::Save(const nlohmann::json::iterator& _iterator)
{
	nlohmann::json json = {
		{"type", type},
		{"position", {position.x, position.y, position.z }},
		{"rotation",{rotation.x, rotation.y, rotation.z, rotation.w }},
		{"euler", {euler.x, euler.y, euler.z }},
		{"scale", {scale.x, scale.y, scale.z }},
		{"local_position", {local_position.x, local_position.y, local_position.z }},
		{"local_rotation",{local_rotation.x, local_rotation.y, local_rotation.z, local_rotation.w }},
		{"local_euler", {local_euler.x, local_euler.y, local_euler.z }},
		{"local_scale", {local_scale.x, local_scale.y, local_scale.z }}
	};

	_iterator.value().push_back(json);
}

void ComponentTransform::Load(const nlohmann::json _json)
{
	type = _json["type"];
	position = float3(_json["position"][0], _json["position"][1], _json["position"][2]);
	rotation = Quat(_json["rotation"][0], _json["rotation"][1], _json["rotation"][2], _json["rotation"][3]);
	euler = float3(_json["euler"][0], _json["euler"][1], _json["euler"][2]);
	scale = float3(_json["scale"][0], _json["scale"][1], _json["scale"][2]);
	local_position = float3(_json["local_position"][0], _json["local_position"][1], _json["local_position"][2]);
	local_rotation = Quat(_json["local_rotation"][0], _json["local_rotation"][1], _json["local_rotation"][2], _json["local_rotation"][3]);
	local_euler = float3(_json["local_euler"][0], _json["local_euler"][1], _json["local_euler"][2]);
	local_scale = float3(_json["local_scale"][0], _json["local_scale"][1], _json["local_scale"][2]);

	GetTransformMatrix();
}

void ComponentTransform::SetLocalPosition(float3 position)
{
	local_position = position;
	GetTransformMatrix();
}

void ComponentTransform::SetLocalScale(float3 _scale)
{
	local_scale = _scale;
	GetTransformMatrix();
}

void ComponentTransform::UpdateLocalTransformMatrix()
{
	local_transform_matrix = GetMyGameObject()->GetParent()->GetTransform()->transform_matrix.Inverted() * transform_matrix;

	local_transform_matrix.Decompose(local_position, local_rotation, local_scale);
}

float3 ComponentTransform::GetLocalRotationToEuler()
{
	return local_rotation.ToEulerXYZ() * RADTODEG;
}

float3 ComponentTransform::TransformEulerAngles()
{
	return local_rotation.ToEulerXYZ();
}

void ComponentTransform::SetLocalRotationFromEuler(float3 euler_rotation)
{
	local_rotation = Quat::FromEulerXYZ(euler_rotation.x * DEGTORAD, euler_rotation.y* DEGTORAD, euler_rotation.z* DEGTORAD);
	local_transform_matrix = float4x4::FromTRS(local_position, local_rotation, local_scale);
	GetTransformMatrix();
}

float4x4 ComponentTransform::GetTransformMatrix()
{
	//local_rotation = Quat::FromEulerXYZ(local_euler.x * DEGTORAD, local_euler.y* DEGTORAD, local_euler.z* DEGTORAD);
	local_transform_matrix = float4x4::FromTRS(local_position, local_rotation, local_scale);

	if (GetMyGameObject()->GetParent())
	{
		transform_matrix = GetMyGameObject()->GetParent()->GetTransform()->transform_matrix * local_transform_matrix;
	}
	else
	{
		transform_matrix = local_transform_matrix;
	}
	for (int i = 0; i < GetMyGameObject()->GetChildren().size(); i++)
	{
		GetMyGameObject()->GetChildren()[i]->GetTransform()->GetTransformMatrix();
	}
	if (GetMyGameObject()->GetMesh() && GetMyGameObject()->GetMesh()->debug_bb)
	{
		GetMyGameObject()->UpdateBB();
	}
	transform_matrix.Decompose(position, rotation, scale);
	return transform_matrix;
}

void ComponentTransform::SetTransformMatrix(float4x4 transform_matrix)
{
	this->transform_matrix = transform_matrix;
	UpdateLocalTransformMatrix();
	GetTransformMatrix();

	transform_matrix.Decompose(position, rotation, scale);
}
