#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "glew\glew.h"
#include "SDL\include\SDL_opengl.h"

#pragma comment (lib, "glew/glew32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
	name = "Renderer3D";
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (ret == true)
	{
		//Use Vsync
		if (VSync && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", __glewErrorStringREGAL(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", __glewErrorStringREGAL(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", __glewErrorStringREGAL(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);

	}
	// Projection matrix for
	OnResize(App->window->GetWindowWidth(), App->window->GetWindowHeight());

	// Initialize glew
	GLenum error = glewInit();
	if (error != GL_NO_ERROR)
	{
		LOG("Error initializing gler library! %s\n", SDL_GetError());
			ret = false;
	}
	else
	{
		LOG("Using Glew %s", glewGetString(GLEW_VERSION));
		LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
	}

	//TEXTURE TEST
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

update_status ModuleRenderer3D::Update(float dt)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	
	if (!meshes.empty())
	{
		for (uint i = 0; i < meshes.size(); ++i)
		{
			Draw(meshes[i]);
			if (normals)
				DrawNormals(meshes[i]);
		}
		
	}

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Load and Save
bool ModuleRenderer3D::LoadDefault(nlohmann::json &load_default_json)
{
	VSync = load_default_json[name]["VSync"];

	return true;
}

bool ModuleRenderer3D::Load(nlohmann::json &load_json)
{
	VSync = load_json[name]["VSync"];

	return true;
}

bool ModuleRenderer3D::Save(nlohmann::json &save_json)
{
	save_json[name]["VSync"] = VSync;

	return true;
}

bool ModuleRenderer3D::GetVSync()
{
	return VSync;
}

void ModuleRenderer3D::SetVSync(bool VSync)
{
	this->VSync = VSync;

	if (this->VSync)
	{
		SDL_GL_SetSwapInterval(1);
	}
	else 
	{
		SDL_GL_SetSwapInterval(0);
	}
}

void ModuleRenderer3D::GLBuffer(CustomMesh *mesh)
{
	glGenBuffers(1, &mesh->id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->n_vertices * 3, mesh->vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &mesh->id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->n_indexes, mesh->indexes, GL_STATIC_DRAW);

	//NORMALS
	mesh->face_normal = new float[mesh->n_indexes];
	for (int i = 0; i < mesh->n_indexes; ++i) {
		mesh->face_normal[i] = (mesh->vertices[i] + mesh->vertices[i + 1] + mesh->vertices[i + 2]) / 3;
	}

	if (mesh->normals != nullptr) {
		glGenBuffers(1, &mesh->id_normal);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normal);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->n_vertices * 3, mesh->normals, GL_STATIC_DRAW);
	}

	// TEXTURE TEST
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &id_texture);
	glBindTexture(GL_TEXTURE_2D, id_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	meshes.push_back(mesh);
}

void ModuleRenderer3D::Draw(CustomMesh *mesh)
{
	glColor3f(1.f, 1.f, 1.f);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);

	// TEXTURE TEST
	glBindTexture(GL_TEXTURE_2D, id_texture);

	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, mesh->n_indexes, GL_UNSIGNED_INT, NULL);

	

}

void ModuleRenderer3D::DrawNormals(CustomMesh *mesh)
{
	if (mesh->normals != nullptr) 
	{
		glColor3f(0.f, 1.f, 0.f);
		glBegin(GL_LINES);

		if (vertex_normals) 
		{
			int j = 0;
			for (int i = 0; i < mesh->n_vertices*3; i += 3) {
				glVertex3f(mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2]);
				glVertex3f(mesh->vertices[i] + mesh->normals[j].x, mesh->vertices[i + 1] + mesh->normals[j].y, mesh->vertices[i + 2] + mesh->normals[j].z);
				++j;
			}
		}
		else {

			for (int i = 0; i < mesh->n_indexes; i += 3) {
				glVertex3f(mesh->face_center_point[i], mesh->face_center_point[i + 1], mesh->face_center_point[i + 2]);
				glVertex3f(mesh->face_center_point[i] + mesh->face_normal[i], mesh->face_center_point[i + 1] + mesh->face_normal[i + 1], mesh->face_center_point[i + 2] + mesh->face_normal[i + 2]);
			}
		}

		glEnd();
	}
}
