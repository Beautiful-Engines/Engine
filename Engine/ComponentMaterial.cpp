#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "glew/glew.h"

ComponentMaterial::ComponentMaterial(GameObject* _game_object) : Component(_game_object, ComponentType::MATERIAL)
{
}

ComponentMaterial::~ComponentMaterial()
{
	glDeleteBuffers(1, &id_texture);
}

void ComponentMaterial::Texture(ComponentMesh* _component_mesh)
{
	if (id_texture > 0) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, _component_mesh->id_uv);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		if (checkered == false)
		{
			glBindTexture(GL_TEXTURE_2D, id_texture);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 1);
		}
	}
}