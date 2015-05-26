#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include <vector>
#include "stdint.h"
#include "Actor.h"
#include "SharedPointer.h"
namespace Engine
{
	struct CollisionInformation																		//Stores information used for resolution
	{
		Vector3 m_Normal;
		float m_TimeOfCollision;
		unsigned int m_ThisID;
		unsigned int m_OtherID;
		bool m_Collision;
		void (*m_CallBack)(SharedPointer<Engine::Actor> ,CollisionInformation );
	};

	struct BoundingBox																				//Extents of Collision body
	{
	private:
		Vector3 m_OffsetToCenter;
		float m_HalfWidth;
		float m_HalfHeight;
		float m_HalfDepth;
	public:
		//Get Functions
		float getHalfWidth()
		{
			return m_HalfWidth;
		}
		float getHalfHeight()
		{
			return m_HalfHeight;
		}
		float getHalfDepth()
		{
			return m_HalfDepth;
		}
		Vector3& getOffset();

		//Set Functions 
		BoundingBox()
		{
		     m_HalfWidth=4;
			 m_HalfHeight=4;
			 m_HalfDepth = 4;
			 m_OffsetToCenter=0;
		}
		void setHalfWidth(const float i_HalfWidth)
		{
			m_HalfWidth=i_HalfWidth;
		}
		void setHalfHeight(const float i_HalfHeight)
		{
			m_HalfHeight=i_HalfHeight;
		}
		void setHalfDepth(const float i_HalfDepth)
		{
			m_HalfDepth = i_HalfDepth;
		}
		void setOffset(const Vector3 
			i_Offset)
		{
			m_OffsetToCenter=i_Offset;
		}
	};
	class CollisionSystem
	{
		BoundingBox m_Collider;
		SharedPointer<Actor> m_Actor;
		unsigned int m_BitMask;
		unsigned int m_CollisionMask;
		static std::vector<CollisionSystem> m_CollidingActors;
		CollisionInformation m_Info;
		static float ResolveCollisions(std::vector<CollisionSystem>,float);
	public:
		CollisionSystem(){};
		CollisionSystem(const CollisionSystem& );
		CollisionSystem(const SharedPointer<Actor> i_Actor, void(*callBack)(SharedPointer<Engine::Actor>, CollisionInformation), const unsigned int i_BitMask,
			const unsigned int i_CollisionMask, const Vector3 i_Offset, const float i_HalfWidth, const float i_HalfHeight,const float i_HalfDepth)
			:m_Actor(i_Actor),m_BitMask(i_BitMask),m_CollisionMask(i_CollisionMask)
		{ 
			m_Collider.setOffset(i_Offset);
			m_Collider.setHalfWidth(i_HalfWidth);
			m_Collider.setHalfHeight(i_HalfHeight);
			m_Collider.setHalfDepth(i_HalfDepth);
			m_Info.m_CallBack=callBack;
			m_CollidingActors.push_back(*this);	
		};
		CollisionInformation& getCollisionInfo();
		inline BoundingBox& getBoundingBox();
		inline void setBoundingBox(const Vector3, const float, const float, const float);
		static inline void addCollidingActor(const CollisionSystem);
		static void deleteActor(const unsigned int);
		static void deleteActors();
		static void checkForDeath();	
		static float checkForCollisions(const float);
		const CollisionSystem& operator=(const CollisionSystem&) ;
	};
}
#include "CollisionSystem.inl"
#endif