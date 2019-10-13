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

void ComponentMaterial::Texture()
{
	if (id_texture > 0) {
		// enable textures
		std::vector<Component*> components = my_game_object->GetComponents();
		std::vector<Component*>::iterator iterator_component = components.begin();


		for (; iterator_component != components.end(); ++iterator_component) {
			if (*iterator_component != nullptr && (*iterator_component)->GetType() == ComponentType::MESH)
			{
				ComponentMesh* mesh;
				mesh = (ComponentMesh*)*iterator_component;

				glEnable(GL_TEXTURE_2D);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glBindTexture(GL_TEXTURE_2D, id_texture);
				// set UV
				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uv);
				glTexCoordPointer(3, GL_FLOAT, 0, NULL);

				glColor3f(1, 1, 1);
			}
		}
		
	}
}