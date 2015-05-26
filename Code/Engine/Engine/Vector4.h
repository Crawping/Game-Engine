#ifndef VECTOR4_H
#define VECTOR4_H
#include "Vector3.h"

namespace Engine
{
	class Vector4
	{
				
		float x, y, z, w;
	public:
		inline Vector4(void) {};												//Default Constructor
		inline Vector4(const float, const float, const float, const float);					//Constructor taking 3 arguments	
		inline Vector4(const Vector4&);											//Copy Constructor
		inline Vector4(const Vector3&);
		
		inline Vector3 toVector3();
		inline float getx() const;												//Get Functions													
		inline float gety() const;
		inline float getz() const;
		inline float getw() const;

		inline void set(const float, const float, const float, const float);									//Combined Set Function


		inline void setx(const float);											//Individual Set Functions
		inline void sety(const float);
		inline void setz(const float);
		inline void setw(const float);

		inline const Vector4 operator=(const Vector4&) ;						//Assignment operator
		inline const Vector4 operator=(const float) ;	
		inline const Vector4 operator+=(const Vector4&) ;						//Operator +=
	};


	inline Vector4 operator+(const Vector4&, const Vector4&);					//Other Operators
	inline Vector4 operator-(const Vector4&, const Vector4&);
	inline float operator*(const Vector4&, const Vector4&);
	inline bool operator==(const Vector4&, const Vector4&);
	inline std::ostream& operator<<(std::ostream&, const Vector4&);		
}

#include "Vector4.inl"

#endif