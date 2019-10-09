#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"

#include "glew\glew.h"
#include <gl/GL.h>
#define PAR_SHAPES_IMPLEMENTATION
#include "Par/par_shapes.h"
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
	/*sphere = par_shapes_create_subdivided_sphere(5);

	s_vertex = 0;
	glGenBuffers(1, &s_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, s_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sphere->npoints * 3, sphere->points, GL_STATIC_DRAW);

	glGenBuffers(1, &s_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T)*sphere->ntriangles * 3, sphere->triangles, GL_STATIC_DRAW);*/
	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float dt)
{
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glBindBuffer(GL_ARRAY_BUFFER, s_vertex);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_index);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//// draw a cube
	//glDrawElements(GL_TRIANGLES, sphere->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);

	//// deactivate vertex arrays after drawing
	//glDisableClientState(GL_VERTEX_ARRAY);

	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	LOG("Cleaning Scene");
	/*par_shapes_free_mesh(sphere);*/
	return true;
}
