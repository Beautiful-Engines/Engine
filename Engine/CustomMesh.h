#ifndef __CustomMesh_H__
#define __CustomMesh_H__

#include "Assimp/include/vector3.h"

#include "Texture.h"

struct CustomMesh
{

	CustomMesh() {};
	~CustomMesh()
	{
		delete[] vertices;
		delete[] indexes;
		delete[] normals;

		vertices = nullptr;
		indexes = nullptr;
		normals = nullptr;
	};


	uint id_index = 0u;  // index in VRAM
	uint n_indexes = 0u;
	uint *indexes = nullptr;

	uint id_vertex = 0u;  // unique vertex in VRAM
	uint n_vertices = 0u;
	float *vertices = nullptr;

	uint id_normal = 0u;
	aiVector3D *normals;

	float *face_center_point = nullptr;
	float *face_normal = nullptr;

	Texture* texture = nullptr;
	uint id_texture = 0u;

}; 

#endif // !__CustomMesh_H__