#define _CRT_SECURE_NO_DEPRECATE
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
#define PAR_SHAPES_IMPLEMENTATION
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
	

	return true;
}


bool ModuleScene::Start()
{
	sphere = par_shapes_create_subdivided_sphere(5);

	s_vertex = 0;
	glGenBuffers(1, &s_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, s_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sphere->npoints * 3, sphere->points, GL_STATIC_DRAW);

	glGenBuffers(1, &s_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T)*sphere->ntriangles * 3, sphere->triangles, GL_STATIC_DRAW);
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
	glBindBuffer(GL_ARRAY_BUFFER, s_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// draw a cube
	glDrawElements(GL_TRIANGLES, sphere->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);

	/*float vertices[] = {
		0.f, 0.f, 0.f,
		-5.f, 0.f, 0.f,
		-5.f, -5.f, 0.f,
		0, -5.f, 0,
		0, -5, -5,
		0, 0, -5,
		-5, 0, -5,
		-5, -5, -5 };
	int indices[] = { 0,1,2, 2,3,0,   // 36 of indices
					 0,3,4, 4,5,0,
					 0,5,6, 6,1,0,
					 1,6,7, 7,2,1,
					 7,4,3, 3,2,7,
					 4,7,6, 6,5,4 };
	uint my_id = 0;
	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8 * 3, vertices, GL_STATIC_DRAW);
	uint my_indices = 0;
	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*36, indices, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// … draw other buffers
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);*/


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