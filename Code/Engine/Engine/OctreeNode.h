#ifndef OCTREENODE_H
#define OCTREENODE_H
#include "Vector3.h"
#include "AABB.h"
#define EPSILON 0.001f;
namespace Engine
{

	//Collision info for a frame
	struct StaticCollisionInfo
	{
		bool m_forwardX;
		bool m_backwardX;
		bool m_forwardZ;
		bool m_backwardZ;
		bool m_groundCheck;
		bool m_camCollided;
		float height;
		float forwardXdistance;
		float forwardZdistance;
		float backwardXdistance;
		float backwardZdistance;
		float camDistance;
		StaticCollisionInfo() :m_forwardX(false), m_backwardX(false), m_forwardZ(false), m_backwardZ(false), m_groundCheck(false), m_camCollided(false)
		{
			height = -10000.0f;
			forwardXdistance=10000.0f;
			forwardZdistance=10000.0f;
			backwardXdistance=10000.0f;
			backwardZdistance = 10000.0f;
			camDistance = 10000.0f;

		};
	};

	//Extents of Collision body
	
	class OctreeNode
	{
		AABB m_AABB;
		OctreeNode* m_children[8];
		Vector3 m_coordinate;
		int m_childCount;
	public:
		OctreeNode() :m_childCount(0){};
		OctreeNode(AABB i_box) :m_AABB(i_box), m_childCount(0){};

		// Setters
		inline void SetAABB(AABB i_box);
		inline void SetCoordinate(Vector3 i_value);
		
		// Getters
		inline Vector3 GetCoordinate();
		inline AABB GetAABB();
		inline OctreeNode* GetChildByIndex(int i_index);

		inline void AddChild(OctreeNode* i_node);
		
		//Checks to see if the ray intersects the AABB for the particular octree node
		virtual int CheckForCollision(Vector3& p0, Vector3& p1, StaticCollisionInfo& i_frameCollisionInfo);
	};
}
#include "OctreeNode.inl"
#endif// OCTREENODE_H