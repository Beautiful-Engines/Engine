#ifndef __CustomMesh_H__
#define __CustomMesh_H__

class CustomMesh
{
public:
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


public:
	uint id_index = 0;  // index in VRAM
	uint n_indexes = 0;
	uint *indexes = nullptr;

	uint id_vertex = 0;  // unique vertex in VRAM
	uint n_vertices = 0;
	float *vertices = nullptr;

	uint id_normal = 0;
	float *normals = nullptr;

	float *face_normal = nullptr;

}; 

#endif // !__CustomMesh_H__