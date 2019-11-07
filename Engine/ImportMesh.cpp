//#include "Application.h"
//#include "ModuleFileSystem.h"
//
//#include "Assimp/include/cimport.h"
//#include "Assimp/include/scene.h"
//#include "Assimp/include/postprocess.h"
//#include "Assimp/include/cfileio.h"
//
//#define PAR_SHAPES_IMPLEMENTATION
//#include "Par/par_shapes.h"
//
//#include "ComponentMesh.h"
//#include "ImportMesh.h"
//
//#pragma comment (lib, "Assimp/libx86/assimp.lib")
//
//ImportMesh::ImportMesh()
//{
//}
//
//
//ImportMesh::~ImportMesh()
//{
//}
//
//bool ImportMesh::Init()
//{
//	return true;
//}
//
//bool ImportMesh::CleanUp()
//{
//	return true;
//}
//
////ComponentMesh* ImportMesh::Load(const char * exported_file)
////{
////	
////}
////
////ComponentMesh* ImportMesh::Import(const aiScene* scene, const aiMesh* mesh)
////{
////	
////}
//
//bool ImportMesh::Save(ComponentMesh* mesh)
//{
//	// amount of indices / vertices / colors / normals / texture_coords / AABB
//	uint ranges[2] = { mesh->n_indexes, mesh->n_vertices };
//	uint size = sizeof(ranges) + sizeof(uint) * mesh->n_indexes + sizeof(float) * mesh->n_vertices * 3;
//
//	// Allocate
//	char* data = new char[size];
//	char* cursor = data;
//
//	// First store ranges
//	uint bytes = sizeof(ranges);
//	memcpy(cursor, ranges, bytes);
//	cursor += bytes;
//
//	// Store indices
//	bytes = sizeof(uint) * mesh->n_indexes;
//	memcpy(cursor, mesh->indexes, bytes);
//	cursor += bytes;
//
//	// Store vertices
//	bytes = sizeof(float3) * mesh->n_vertices;
//	memcpy(cursor, mesh->vertices, bytes);
//	cursor += bytes;
//
//	//if (mesh->total_normal > 0)
//	//{
//	//	//Store normal
//	//	bytes = sizeof(float3) * mesh->total_normal;
//	//	memcpy(cursor, mesh->normal, bytes);
//	//	cursor += bytes;
//	//}
//	//if (mesh->total_uv > 0)
//	//{
//	//	//Store uv
//	//	bytes = sizeof(float2) * mesh->total_uv;
//	//	memcpy(cursor, mesh->uv, bytes);
//	//	cursor += bytes;
//	//}
//
//	uint ret = App->file_system->Save(std::string(LIBRARY_MESH_FOLDER"mesh.spk").c_str(), data, size);
//	RELEASE_ARRAY(data);
//
//	return true;
//}
//
//
////ComponentMesh* ImportMesh::LoadPrimitive(PRIMITIVE_TYPE type)
////{
////	
////}