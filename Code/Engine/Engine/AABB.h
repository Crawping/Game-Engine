#ifndef AABB_H
#define AABB_H
#include "Vector3.h"
namespace Engine
{
	struct AABB
	{
	private:
		Vector3 m_Center;
		Vector3 m_Widths;
		float m_HalfWidth;
		float m_HalfHeight;
		float m_HalfDepth;
	public:
		//Get Functions
		inline float getHalfWidth();
		inline float getHalfHeight();
		inline float getHalfDepth();
		inline Vector3 getWidths();
		inline Vector3 getCenter();

		//Set Functions 
		AABB(){}
		inline AABB(Vector3 i_center, float i_dimensions);
	};

}
#include "AABB.inl"
#endif //AABB_H