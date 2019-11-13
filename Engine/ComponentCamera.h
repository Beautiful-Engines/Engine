#ifndef __ComponentCamera_H__
#define __ComponentCamera_H__

#include "Component.h"

#include "MathGeoLib\include\MathBuildConfig.h"
#include "MathGeoLib\include\Geometry\GeometryAll.h"
#include "MathGeoLib\include\Math\MathAll.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* _game_object);
	~ComponentCamera();

	Frustum frustum;

	Plane GetFrustumNearPlane();
	float GetFrustumNearPlaneDistance();
	Plane GetFrustumFarPlane();
	float GetFrustumFarPlaneDistance();

	void SetFrustumNearPlaneDistance(float distance);
	void SetFrustumFarPlaneDistance(float distance);

	void SetFrustumFOV(float fov, bool degrees = false);

	float GetFrustumHorizontalFOV(bool degrees = false);
	float GetFrustumVerticalFOV(bool degrees = false);
	float GetFrustumAspectRatio();

	void LookAt(float3 position);

	float4x4 GetOpenGLViewMatrix();
	float4x4 GetOpenGLProjectionMatrix();

	void UpdateFrustumTransform();
	void DrawFrustum();

	bool active = false;

private:

	void Update(float dt);

	

};
#endif
