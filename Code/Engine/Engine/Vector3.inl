#include <iostream>
#include <assert.h>
#include "Utils.h"
#include "D3dx9math.h"
#include <math.h>

inline Engine::Vector3::Vector3(const Engine::Vector3 &a)
{
	x = a.getx();
	y = a.gety();
	z = a.getz();
	assert(!isNAN(x));
	assert(!isNAN(y));
	assert(!isNAN(z));
}

inline Engine::Vector3::Vector3(const float a, const float b, const float c)
{
	x = a;
	y = b;
	z = c;
	assert(!isNAN(x));
	assert(!isNAN(y));
	assert(!isNAN(z));
}

inline float Engine::Vector3::getx() const
{
	assert(!isNAN(x));
	return(x);
}

inline float Engine::Vector3::gety() const
{
	assert(!isNAN(y));
	return(y);
}

inline float Engine::Vector3::getz() const
{
	assert(!isNAN(z));
	return(z);
}

inline D3DXVECTOR3 Engine::Vector3::getD3DVector3() const
{
	D3DXVECTOR3 Vector3InD3D;
	Vector3InD3D.x = x;
	Vector3InD3D.y = y;
	Vector3InD3D.z = z;
	return Vector3InD3D;

}

inline void Engine::Vector3::set(const float i_x, const float i_y, const float i_z)
{
	x = i_x;
	y = i_y;
	z = i_z;
	assert(!isNAN(x));
	assert(!isNAN(y));
	assert(!isNAN(z));
}


inline void Engine::Vector3::setx(const float i_x)
{
	x = i_x;
	assert(!isNAN(x));
}

inline void Engine::Vector3::sety(const float i_y)
{
	y = i_y;
	assert(!isNAN(y));
}

inline void Engine::Vector3::setz(const float i_z)
{
	z = i_z;
	assert(!isNAN(z));
}

inline Engine::Vector3 Engine::Vector3::normalized() const
{
	float magnitude = sqrt(x*x + y*y + z*z);
	return Vector3(x / magnitude, y / magnitude, z / magnitude);
}

inline const Engine::Vector3 Engine::Vector3::operator=(const Engine::Vector3& i_Vector)
{
	x = i_Vector.getx();
	y = i_Vector.gety();
	z = i_Vector.getz();
	assert(!isNAN(x));
	assert(!isNAN(y));
	assert(!isNAN(z));
	Engine::Vector3 temp = *this;
	return(temp);
}

inline const Engine::Vector3 Engine::Vector3::operator=(const float i_rhs)
{
	x = i_rhs;
	y = i_rhs;
	z = i_rhs;
	assert(!isNAN(x));
	assert(!isNAN(y));
	assert(!isNAN(z));
	Engine::Vector3 temp = *this;
	return(temp);
}

inline const Engine::Vector3 Engine::Vector3::operator+=(const Engine::Vector3& i_Vector)
{

	x = x + i_Vector.getx();
	y = y + i_Vector.gety();
	z = z + i_Vector.getz();
	Engine::Vector3 temp = *this;
	return(temp);
}


/*
inline Engine::Vector3 operator-(const Engine::Vector3& lhs, const Engine::Vector3& rhs)
{
Engine::Vector3 temp;
temp.setx(lhs.getx() - rhs.getx());
temp.sety(lhs.gety() - rhs.gety());
temp.setz(lhs.getz() - rhs.getz());
return temp;
}*/

inline float Engine::operator*(const Engine::Vector3& lhs, const Engine::Vector3& rhs)
{
	return(lhs.getx()*rhs.getx() + lhs.gety()*rhs.gety() + lhs.getz()*rhs.getz());
}

inline const Engine::Vector3 Engine::operator*(const Vector3& i_Vector, const float i_float)
{
	return Vector3(i_Vector.getx()*i_float, i_Vector.gety()*i_float, i_Vector.getz()*i_float);
}

inline bool Engine::operator==(const Engine::Vector3& lhs, const Engine::Vector3& rhs)
{
	if (!Engine::compareFloats(lhs.getx(), rhs.getx(), 5) || !Engine::compareFloats(lhs.getx(), rhs.getx(), 5) || !Engine::compareFloats(lhs.getx(), rhs.getx(), 5))
		return false;
	else
		return true;
}

inline std::ostream& Engine::operator<<(std::ostream& i_out, const Engine::Vector3& rhs)
{
	i_out << "( " << rhs.getx() << " , " << rhs.gety() << " , " << rhs.getz() << " )";
	return i_out;
}

inline const Engine::Vector3 operator/(const Engine::Vector3& lhs, const float rhs)
{
	assert(rhs != 0.0f);
	return Engine::Vector3(lhs.getx() / rhs , lhs.gety()/rhs , lhs.getz()/rhs);
}

inline const Engine::Vector3 Engine::Vector3::cross(const Engine::Vector3& rhs)
{
	Engine::Vector3 temp;
	temp.setx(y*rhs.getz() - z*rhs.gety());
	temp.sety(z*rhs.getx() - x*rhs.getz());
	temp.setz(x*rhs.gety() - y*rhs.getx());
	return temp;
}

inline const Engine::Vector3 Engine::Vector3::operator-()
{
	Engine::Vector3 temp(-x,-y,-z);
	return temp;
}
