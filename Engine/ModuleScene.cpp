#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"


#include "glew\glew.h"
#include <gl/GL.h>
#include "SDL\include\SDL_opengl.h"
#include "PCG\pcg_extras.hpp"
#include "PCG\pcg_random.hpp"
#include "PCG\pcg_uint128.hpp"
#include "MathGeoLib\include\MathGeoLib.h"
#include "par_shapes.h"
#include <random>

#pragma comment (lib, "glew/glew32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
}


ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	// Initialize ImGUi
	LOG("Creating Scene");

	sphere = par_shapes_create_subdivided_sphere(5);

	s_id = 0;
	glGenBuffers(1, (GLuint*) &(s_id));
	glBindBuffer(GL_ARRAY_BUFFER, s_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*sphere->npoints * 3, sphere->points, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(s_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T)*sphere->ntriangles * 3, sphere->tcoords, GL_STATIC_DRAW);

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

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, s_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, sphere->ntriangles * 3, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	LOG("Cleaning Scene");
	par_shapes_free_mesh(sphere);
	return true;
}

void ModuleScene::IntersectionTest()
{
	//// RANDOM CREATION TEST
	//// Choose a random int
	//pcg_extras::seed_seq_from<std::random_device> seed_source_int;
	//pcg32 rn_int(seed_source_int);
	//std::uniform_int_distribution<int> uniform_int(0, 100);
	//int random_int = uniform_int(rn_int);
	//LOG("number int is: %i", random_int);

	//// Choose a random float
	//pcg_extras::seed_seq_from<std::random_device> seed_source_float;
	//pcg32 rn_float(seed_source_float);
	//std::uniform_real_distribution<float> uniform_float(0, 1);
	//float random_float = uniform_float(rn_float);
	//LOG("number float is: %f", random_float);


	//// MATH INTERSECTS TEST
	//LCG random;

	//math::Sphere sphere({ random.Float(0, 100) ,random.Float(0, 100) ,random.Float(0, 100) }, random.Float(0, 100));
	//math::Cylinder cylinder({ random.Float(0, 100) ,random.Float(0, 100) ,random.Float(0, 100) }, { random.Float(0, 100) ,random.Float(0, 100) ,random.Float(0, 100) }, random.Float(0, 100));
	//math::Capsule capsule({ random.Float(0, 100) ,random.Float(0, 100) ,random.Float(0, 100) }, { random.Float(0, 100) ,random.Float(0, 100) ,random.Float(0, 100) }, random.Float(0, 100));
	//math::AABB aabb(sphere);
	//math::Ray ray({ random.Float(0, 100) ,random.Float(0, 100) ,random.Float(0, 100) }, { random.Float(0, 100) ,random.Float(0, 100) ,random.Float(0, 100) });
	//ray.dir.Normalize();

	//if (sphere.Intersects(capsule))	LOG("SPHERE AND CAPSULE INTERSECTION");
	//if (capsule.Intersects(ray)) LOG("CAPSULE AND RAY INTERSECTION");
	//if (ray.Intersects(sphere))	LOG("RAY AND SPHERE INTERSECTION");

	//// TESTING COLOR
	//glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//glClearColor(0.4f, 0.2f, 0.4f, 0.0f);

}