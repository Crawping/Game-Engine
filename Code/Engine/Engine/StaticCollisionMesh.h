#ifndef STATIC_COLLISION_MESH_H
#define STATIC_COLLISION_MESH_H

#include "Vector3.h"
#include <stdint.h>

namespace Engine
{
	

	class StaticCollisionMesh
	{
		unsigned int m_numberOfVertices;
		Vector3* m_vertices;
	public:
		StaticCollisionMesh() {}
		unsigned int GetNumberOfVertices(){ return m_numberOfVertices; }
		Vector3* GetVertices(){ return m_vertices; }
		bool LoadMeshDataFromFile(const char* i_path, HWND i_WindowHandle);
	};
}
#endif //STATIC_COLLISION_MESH_H