#ifndef VECTOR3_H
#define VECTOR3_H
#include <ostream>
#include "D3dx9math.h"

namespace Engine
{
	class Vector3
	{

		float x, y, z;
	public:
		inline Vector3(void) {};												//Default Constructor
		inline Vector3(const float, const float, const float);					//Constructor taking 3 arguments	
		inline Vector3(const Vector3&);											//Copy Constructor
		inline float getx() const;												//Get Functions													
		inline float gety() const;
		inline float getz() const;
		inline D3DXVECTOR3 getD3DVector3() const;
		inline void set(const float, const float, const float);									//Combined Set Function


		inline void setx(const float);											//Individual Set Functions
		inline void sety(const float);
		inline void setz(const float);


		inline Vector3 normalized() const;

		inline const Vector3 operator=(const Vector3&);						//Assignment operator
		inline const Vector3 operator=(const float);
		inline const Vector3 operator+=(const Vector3&);						//Operator +=
		inline const Vector3 cross(const Vector3&);
		inline const Vector3 operator-();
	};


	inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs)				//Other Operators
	{
			Engine::Vector3 temp;
			temp.setx(lhs.getx() + rhs.getx());
			temp.sety(lhs.gety() + rhs.gety());
			temp.setz(lhs.getz() + rhs.getz());
			return temp;
	}
	inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
	{

		Engine::Vector3 temp;
		temp.setx(lhs.getx() - rhs.getx());
		temp.sety(lhs.gety() - rhs.gety());
		temp.setz(lhs.getz() - rhs.getz());
		return temp;

	}
	
	inline float operator*(const Vector3&, const Vector3&);
	inline const Vector3 operator*(const Vector3&, const float);
	inline const Vector3 operator/(const Vector3&, const float);
	inline bool operator==(const Vector3&, const Vector3&);
	inline std::ostream& operator<<(std::ostream&, const Vector3&);
}

#include "Vector3.inl"

#endif