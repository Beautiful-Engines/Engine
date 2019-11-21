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

	void Save(const nlohmann::json::iterator& _iterator);
	void Load(const nlohmann::json _json);

	Frustum frustum;

	Plane GetNearPlane();
	float GetNearPlaneDistance();
	Plane GetFarPlane();
	float GetFarPlaneDistance();

	void SetNearPlaneDistance(float distance);
	void SetFarPlaneDistance(float distance);

	void SetFOV(float fov, bool degrees = false);

	float GetHorizontalFOV(bool degrees = false);
	float GetVerticalFOV(bool degrees = false);
	float GetAspectRatio();

	void LookAt(float3 position);

	float4x4 GetOpenGLViewMatrix();
	float4x4 GetOpenGLProjectionMatrix();

	void UpdateFrustumTransform();
	void DrawFrustum();

	bool update_camera_projection = false;
	bool frustum_culling = false;
	bool main_camera = false;

};
#endif
