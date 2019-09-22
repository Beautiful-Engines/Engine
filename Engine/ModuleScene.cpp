#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"

// TODO Delete later
#include <gl/GL.h>
#include "PCG\pcg_extras.hpp"
#include "PCG\pcg_random.hpp"
#include "PCG\pcg_uint128.hpp"
#include "MathGeoLib\include\MathGeoLib.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	// Initialize ImGUi
	LOG("Creating Scene");

	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float dt)
{
	// TESTING INTERSECTION
	IntersectionTest();

	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	LOG("Cleaning Scene");

	return true;
}

void ModuleScene::IntersectionTest()
{
	//TODO try PCG Family random number
	pcg32  myRNG;
	LCG random;

	math::Sphere sphere({ random.Float(0, 100) ,random.Float(0, 100) ,random.Float(0, 100) }, random.Float(0, 100));
	math::Cylinder cylinder({ random.Float(0, 100) ,random.Float(0, 100) ,random.Float(0, 100) }, { random.Float(0, 100) ,random.Float(0, 100) ,random.Float(0, 100) }, random.Float(0, 100));
	math::Capsule capsule({ random.Float(0, 100) ,random.Float(0, 100) ,random.Float(0, 100) }, { random.Float(0, 100) ,random.Float(0, 100) ,random.Float(0, 100) }, random.Float(0, 100));
	math::AABB aabb(sphere);
	math::Ray ray({ random.Float(0, 100) ,random.Float(0, 100) ,random.Float(0, 100) }, { random.Float(0, 100) ,random.Float(0, 100) ,random.Float(0, 100) });
	ray.dir.Normalize();

	if (sphere.Intersects(capsule))	LOG("SPHERE AND CAPSULE INTERSECTION");
	if (capsule.Intersects(ray)) LOG("CAPSULE AND RAY INTERSECTION");
	if (ray.Intersects(sphere))	LOG("RAY AND SPHERE INTERSECTION");

	// TESTING COLOR
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(0.4f, 0.2f, 0.4f, 0.0f);

}