#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"


ComponentCamera::ComponentCamera(GameObject* _game_object) : Component(_game_object, ComponentType::CAMERA)
{
	frustum.pos = { 0.f, 0.f, 0.f };
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 200.0f;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	SetFrustumFOV(90);
}


ComponentCamera::~ComponentCamera()
{
}

Plane ComponentCamera::GetFrustumNearPlane()
{
	return frustum.NearPlane();
}

float ComponentCamera::GetFrustumNearPlaneDistance()
{
	return frustum.nearPlaneDistance;
}

Plane ComponentCamera::GetFrustumFarPlane()
{
	return frustum.FarPlane();
}

float ComponentCamera::GetFrustumFarPlaneDistance()
{
	return frustum.farPlaneDistance;
}

void ComponentCamera::SetFrustumNearPlaneDistance(float distance)
{
	frustum.nearPlaneDistance = distance;
}

void ComponentCamera::SetFrustumFarPlaneDistance(float distance)
{
	frustum.farPlaneDistance = distance;
}

void ComponentCamera::SetFrustumFOV(float fov, bool degrees)
{
	if (degrees)
		frustum.verticalFov = DegToRad(fov);
	else frustum.verticalFov = fov;

	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * App->window->GetWindowWidth() / App->window->GetWindowHeight());

}

float ComponentCamera::GetFrustumHorizontalFOV(bool degrees)
{
	if (degrees)
		return (RadToDeg(frustum.horizontalFov));
	else
		return frustum.horizontalFov;
}

float ComponentCamera::GetFrustumVerticalFOV(bool degrees)
{
	if (degrees)
		return RadToDeg(frustum.verticalFov);
	else
		return frustum.verticalFov;
}

float ComponentCamera::GetFrustumAspectRatio()
{
	return frustum.AspectRatio();
}

void ComponentCamera::LookAt(float3 position)
{

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
	frustum.pos = GetMyGameObject()->GetTransform()->transform_matrix.TranslatePart();
}

void ComponentCamera::DrawFrustum()
{
	float3 corners[8];
	frustum.GetCornerPoints(corners);
	App->renderer3D->DebugDrawCube(corners, { 1.f, 1.f, 1.f, 1.f });
}

void ComponentCamera::Update(float dt)
{
	DrawFrustum();
}