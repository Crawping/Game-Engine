#ifndef STATICOBJECT_COLLISION_SYSTEM
#define STATICOBJECT_COLLISION_SYSTEM

#include "StaticCollisionMesh.h"
#include "SharedPointer.h"
#include "CollisionOctree.h"
namespace Engine
{

	
	class StaticObjectCollisionSystem
	{
	private:
		HWND m_mainWindow;
		CollisionOctree m_collisionTree;
		StaticCollisionMesh m_collisionMesh[4][4][4];
	public:
		bool Initialize(HWND i_window);
		static StaticObjectCollisionSystem* GetInstance();
		bool LoadStaticCollisionMesh();
		void GetCollidedAABBs(Vector3 i_position, Vector3 i_offset, std::vector<AABB>* i_collidedAABBs, std::vector<D3DCOLOR>* i_colors);
		StaticCollisionInfo CheckForCollision(Engine::Vector3& i_position, Engine::Vector3 i_offset);

	};
}
#endif