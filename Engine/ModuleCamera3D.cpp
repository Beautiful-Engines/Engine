#include "Application.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ModuleTimeManager.h"
#include "ModuleWindow.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	name = "camera3D";

	CalculateViewMatrix();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 0.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	camera = new ComponentCamera(nullptr);
	camera->Enable();
	camera->frustum.pos = { 2.f, 5.f, 0.f };
	LookAt({ 0.f, 0.f, 0.f });

	App->renderer3D->camera = camera;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	App->renderer3D->camera = nullptr;

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	newPos = { 0, 0, 0 };
	float speed = 40.0f * App->timemanager->GetRealDt();
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = speed * 2;

	if (App->input->GetMouseZ() > 0) newPos += camera->frustum.front * speed;
	if (App->input->GetMouseZ() < 0) newPos -= camera->frustum.front * speed;

	// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += camera->frustum.front * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= camera->frustum.front * speed;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= camera->frustum.WorldRight() * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += camera->frustum.WorldRight() * speed;

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.5f;

		Position -= Reference;

		if (dx != 0 || dy != 0)
		{
			float3 vector = camera->frustum.pos - Reference;

			Quat quat_y(camera->frustum.up, dx * 0.003);
			Quat quat_x(camera->frustum.WorldRight(), dy * 0.003);

			vector = quat_x.Transform(vector);
			vector = quat_y.Transform(vector);

			camera->frustum.pos = vector + Reference;
			LookAt(Reference);

			//Position = Reference + Z * length(Position);
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		if (App->scene->GetSelected() != nullptr)
		{
			mdistance;
			LookAt(App->scene->GetSelected()->GetTransform()->position);
			focus = true;
			/*camera_inputs_active = false;*/

			if (App->scene->GetSelected()->GetMesh())
				mdistance = App->scene->GetSelected()->GetDistance() * 5;
			else mdistance = 5;

		}
			/*LookAt({ App->scene->GetSelected()->GetTransform()->local_position.x, App->scene->GetSelected()->GetTransform()->local_position.y, App->scene->GetSelected()->GetTransform()->local_position.z });
			if (App->scene->GetSelected()->GetMesh() && App->scene->GetSelected()->GetMesh()->GetResourceMesh()->is_primitive == false)
			{
				focus = true;
			}
			else
			{
				focus = false;
			}*/
		//}
		
	}
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		CalculateViewMatrix();

		X = float3(1.0f, 0.0f, 0.0f);
		Y = float3(0.0f, 1.0f, 0.0f);
		Z = float3(0.0f, 0.0f, 1.0f);

		Position = float3(0.0f, 0.0f, 5.0f);
		Reference = float3(0.0f, 0.0f, 0.0f);
	}
	if (focus)
	{
		Focus(speed/2);
	}

	/*camera->frustum.pos += newPos;
	Reference += newPos;*/
	
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (App->scene->GetSelected() != nullptr)
		{
			LookAt({ App->scene->GetSelected()->GetTransform()->local_position });

			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Position -= Reference;

			if (dx != 0 || dy != 0)
			{
				float3 vector = camera->frustum.pos - Reference;

				Quat quat_y(camera->frustum.up, dx * 0.003);
				Quat quat_x(camera->frustum.WorldRight(), dy * 0.003);

				vector = quat_x.Transform(vector);
				vector = quat_y.Transform(vector);

				camera->frustum.pos = vector + Reference;
				LookAt(Reference);
			}
		}
	}
	camera->frustum.pos += newPos;
	Reference += newPos;

	return UPDATE_CONTINUE;
}
void ModuleCamera3D::Focus(float speed)
{
	int ratio = 6;
	int distance_ratio = 2;

		if (App->scene->GetSelected() && App->scene->GetSelected()->GetMesh())
		{
			//float go_height = App->scene->GetGameObjects()[i]->GetMesh()->GetMaxPoint().y - App->scene->GetGameObjects()[i]->GetMesh()->GetMinPoint().y;
			float3 end_position = { Reference.x, Reference.y, Reference.z };
			float3 position = { camera->frustum.pos.x, camera->frustum.pos.y, camera->frustum.pos.z };
			float distance = position.Distance({ App->scene->GetSelected()->GetTransform()->local_position.x, App->scene->GetSelected()->GetTransform()->local_position.y, App->scene->GetSelected()->GetTransform()->local_position.z });
			speed = speed * distance;
			if (distance*ratio < mdistance-mdistance/distance_ratio || distance*ratio > mdistance+mdistance/distance_ratio) {
				if (distance < mdistance+5) 
				{
					newPos -= camera->frustum.front * speed/10;
					if (distance + camera->frustum.pos.z >= mdistance)
					{
						focus = false;
					}
				}
				else if (distance > mdistance-5) 
				{
					newPos += camera->frustum.front * speed/10;
					if (distance - camera->frustum.pos.z <= mdistance)
					{
						focus = false;
					}
				}
			}
			else
			{
				focus = false;
			}

			/*if (go_height * go_height_ratio < distance - distance / distance_ratio) {
				newPos -= Z * speed;
			}
			else if (go_height * go_height_ratio > distance + distance / distance_ratio)
			{
				newPos += Z * speed;
			}
			else if (go_height * go_height_ratio < distance + distance / distance_ratio && go_height * go_height_ratio > distance - distance / distance_ratio)
			{
				focus = false;
			}*/
		}
		else
		{
			focus = false;
		}
}
// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;


	camera->LookAt(Reference);
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const float3 &Spot)
{
	camera->LookAt(Spot);
	Reference = Spot;
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	camera->frustum.pos += Movement;
	Reference += Movement;

	//CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	//ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	//ViewMatrixInverse = inverse(ViewMatrix);
}