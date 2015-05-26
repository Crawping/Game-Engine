#ifndef OCTREELEAF_H
#define OCTREELEAF_H

#include "StaticCollisionMesh.h"
#include "OctreeNode.h"
#include "Vector3.h"
namespace Engine
{
	class OctreeLeaf :public OctreeNode
	{
		StaticCollisionMesh m_sectorMeshes;
	public:
		OctreeLeaf(AABB i_box) :OctreeNode(i_box){ }

		// Checks to see if the ray collides with a particular triangle
		int IntersectSegmentTriangle(Engine::Vector3 p, Engine::Vector3 q, Engine::Vector3 a, Engine::Vector3 b, Engine::Vector3 c,
			float &u, float &v, float &w, float &t);

		// Checks to see if the ray collides with any of the mesh triangles in a particular octree leaf sector
		virtual int CheckForCollision(Vector3& i_position, Vector3& i_end, StaticCollisionInfo& currentFrameCollisionStatus);


		StaticCollisionMesh GetMeshes();
		bool LoadMesh(const char* i_path, HWND i_window);
	
	};
}

#endif// OCTREELEAF_H