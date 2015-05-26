#ifndef COLLISIONOCTREE_H
#define COLLISIONOCTREE_H
#include "OctreeNode.h"
#include "OctreeLeaf.h"
#include <string>
#include <vector>

namespace Engine
{
	
	class CollisionOctree
	{
		OctreeNode m_root;
	public:
		
		//Checks to see if the ray intersects with the AABB of a node, and if so, calls this function recursively on its children
		void CheckForCollision(Vector3& i_position, Vector3& i_offset, StaticCollisionInfo& i_frameCollisionInfo, OctreeNode* i_node);

		void GetCollidedAABB(Vector3 i_position, Vector3 i_offset,int depth, StaticCollisionInfo& i_frameCollisionInfo, OctreeNode* i_node, std::vector<AABB>* i_collidedAABBs, std::vector<D3DCOLOR>* i_colors);
		void GetAABB(Vector3 i_position, Vector3 i_offset, int depth,StaticCollisionInfo& i_frameCollisionInfo, OctreeNode* i_node, std::vector<AABB>* i_collidedAABBs, std::vector<D3DCOLOR>* i_colors);

		OctreeNode* GetRoot();

		//Initializes the AABB information for the root
		bool InstantiateRoot(int i_depth, HWND i_window);

		//Creates and initializes the octree
		bool InitializeNodes(int i_depth, OctreeNode* i_node, HWND i_window);
	
	};
}
#endif //COLLISIONOCTREE_H