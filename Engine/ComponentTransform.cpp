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

float3 ComponentTransform::TransformEulerAngles()
{
	return local_rotation.ToEulerXYZ();
}

float4x4 ComponentTransform::GetTransformMatrix()
{
	local_rotation = Quat::FromEulerXYZ(local_euler.x * DEGTORAD, local_euler.y* DEGTORAD, local_euler.z* DEGTORAD);
	local_transform_matrix = float4x4::FromTRS(local_position, local_rotation, local_scale);

	if (GetMyGameObject()->GetParent())
	{
		//std::string a = GetMyGameObject()->GetName();
		//int x = GetMyGameObject()->GetChildren().size();
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

	transform_matrix.Decompose(position, rotation, scale);
	return transform_matrix;
}