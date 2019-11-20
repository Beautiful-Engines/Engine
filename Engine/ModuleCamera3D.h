#ifndef __ModuleCamera3D_H__
#define __ModuleCamera3D_H__

#include "Module.h"
#include "glmath.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "MathGeoLib\include\Math\float3.h"

class ComponentCamera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	void Focus(float speed);
	bool CleanUp();

	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);
	float* GetViewMatrix();
	bool focus = false;
	float3 newPos = { 0, 0, 0 };
	std::vector<float3> lines;

private:

	void CalculateViewMatrix();

public:

	float3 X, Y, Z, Position, Reference;

private:

	ComponentCamera * camera;
	mat4x4 ViewMatrix, ViewMatrixInverse;
};

#endif // !__ModuleCamera3D_H__