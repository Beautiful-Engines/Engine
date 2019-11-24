#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"


ComponentCamera::ComponentCamera(GameObject* _game_object) : Component(_game_object, ComponentType::CAMERA)
{
	enabled = false;
	frustum.pos = { 0.f, 0.f, 0.f };
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.nearPlaneDistance = 2.0f;
	frustum.farPlaneDistance = 200.0f;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	SetFOV(60,true);
}


ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Save(const nlohmann::json::iterator& _iterator)
{
	nlohmann::json json = {
		{"type", type},
		{"enabled",enabled},
		{"update_camera_projection", update_camera_projection},
		{"frustum_culling", frustum_culling },
		{"main_camera", main_camera }
	};

	_iterator.value().push_back(json);
}

void ComponentCamera::Load(const nlohmann::json _json)
{
	type = _json["type"];
	enabled = _json["enabled"];
	update_camera_projection = _json["update_camera_projection"];
	frustum_culling = _json["frustum_culling"];
	main_camera = _json["main_camera"];
}

Plane ComponentCamera::GetNearPlane()
{
	return frustum.NearPlane();
}

float ComponentCamera::GetNearPlaneDistance()
{
	return frustum.nearPlaneDistance;
}

Plane ComponentCamera::GetFarPlane()
{
	return frustum.FarPlane();
}

float ComponentCamera::GetFarPlaneDistance()
{
	return frustum.farPlaneDistance;
}

void ComponentCamera::SetNearPlaneDistance(float distance)
{
	frustum.nearPlaneDistance = distance;
}

void ComponentCamera::SetFarPlaneDistance(float distance)
{
	frustum.farPlaneDistance = distance;
}

void ComponentCamera::SetFOV(float fov, bool degrees)
{
	if (degrees)
		frustum.verticalFov = DegToRad(fov);
	else frustum.verticalFov = fov;

	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * App->window->GetWindowWidth() / App->window->GetWindowHeight());
	update_camera_projection = true;
}

float ComponentCamera::GetHorizontalFOV(bool degrees)
{
	if (degrees)
		return (RadToDeg(frustum.horizontalFov));
	else
		return frustum.horizontalFov;
}

float ComponentCamera::GetVerticalFOV(bool degrees)
{
	if (degrees)
		return RadToDeg(frustum.verticalFov);
	else
		return frustum.verticalFov;
}

float ComponentCamera::GetAspectRatio()
{
	return frustum.AspectRatio();
}

void ComponentCamera::LookAt(float3 position)
{
	float3 vector = position - frustum.pos;

	float3x3 matrix = float3x3::LookAt(frustum.front, vector.Normalized(), frustum.up, float3::unitY);

	frustum.front = (matrix.MulDir(frustum.front).Normalized());
	frustum.up = (matrix.MulDir(frustum.up).Normalized());
}

float4x4 ComponentCamera::GetOpenGLViewMatrix()
{
	float4x4 matrix = frustum.ViewMatrix();
	return matrix.Transposed();
}

float4x4 ComponentCamera::GetOpenGLProjectionMatrix()
{
	return frustum.ProjectionMatrix().Transposed();
}

void ComponentCamera::UpdateFrustumTransform()
{
	float4x4 transform_matrix;
	if (GetMyGameObject())
		transform_matrix = GetMyGameObject()->GetTransform()->transform_matrix;
	else 
		transform_matrix = float4x4::identity;
	frustum.pos = transform_matrix.TranslatePart();
	frustum.up = transform_matrix.WorldY();
	frustum.front = transform_matrix.WorldZ();
}

void ComponentCamera::DrawFrustum()
{
	float3 corners[8];
	frustum.GetCornerPoints(corners);
	App->renderer3D->DebugDrawCube(corners, { 1.f, 1.f, 1.f, 1.f });
}
