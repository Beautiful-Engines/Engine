#include "Application.h"
#include "GameObject.h"
#include "ModuleImport.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"

#include "glew/glew.h"

ComponentTexture::ComponentTexture(GameObject* _game_object) : Component(_game_object, ComponentType::TEXTURE)
{
}

ComponentTexture::~ComponentTexture()
{
	glDeleteBuffers(1, &id_texture);
	id_texture = 0;
}

// Save
void ComponentTexture::Save(const nlohmann::json::iterator& _iterator)
{
	nlohmann::json json = {
		{"type", type},
		{"id_texture", id_texture },
		{"width",width},
		{"height", height},
		{"path", path }
	};

	_iterator.value().push_back(json);
}

void ComponentTexture::DrawTexture(ComponentMesh* _component_mesh)
{
	if (id_texture > 0 && _component_mesh->n_uv > 0) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		
		glBindBuffer(GL_ARRAY_BUFFER, _component_mesh->id_uv);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		
		glBindTexture(GL_TEXTURE_2D, id_texture);
	}
}