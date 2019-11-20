#include "Application.h"
#include "GameObject.h"
#include "ModuleImport.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ComponentTexture.h"

#include "glew/glew.h"

ComponentTexture::ComponentTexture(GameObject* _game_object) : Component(_game_object, ComponentType::TEXTURE)
{
}

ComponentTexture::~ComponentTexture()
{
	if (texture != nullptr)
	{
		glDeleteBuffers(1, &texture->id_texture);
		glDeleteBuffers(1, &default_texture->id_texture);
		texture->id_texture = 0;
		default_texture->id_texture = 0;
	}
}

// Save
void ComponentTexture::Save(const nlohmann::json::iterator& _iterator)
{
	nlohmann::json json = {
		{"type", type},
	};

	_iterator.value().push_back(json);
}

void ComponentTexture::Load(const nlohmann::json _json)
{
	type = _json["type"];
}

void ComponentTexture::DrawTexture(ResourceMesh* _resource_mesh)
{
	if (_resource_mesh->n_uv > 0) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		
		glBindBuffer(GL_ARRAY_BUFFER, _resource_mesh->id_uv);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		
		if(!checkered && texture != nullptr)
			glBindTexture(GL_TEXTURE_2D, texture->id_texture);
		else
			glBindTexture(GL_TEXTURE_2D, default_texture->id_texture);
	}
}