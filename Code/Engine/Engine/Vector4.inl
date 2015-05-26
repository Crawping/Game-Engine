#include <iostream>
#include <assert.h>
#include "Utils.h"
inline Engine::Vector4::Vector4(const Engine::Vector4 & i_Vector)
{
	x = i_Vector.getx();
	y = i_Vector.gety();
	z = i_Vector.getz();
	w = i_Vector.getw();
	assert(!isNAN(x));			
	assert(!isNAN(y));
	assert(!isNAN(z));
	assert(!isNAN(w));
}

inline Engine::Vector4::Vector4(const Engine::Vector3& i_Vector)
{
	x = i_Vector.getx();
	y = i_Vector.gety();
	z = i_Vector.getz();
	w = 0.0f;
	assert(!isNAN(x));
	assert(!isNAN(y));
	assert(!isNAN(z));
}
inline Engine::Vector4::Vector4(const float i_X, const float i_Y, const float i_Z, const float i_W)
{
	x = i_X;
	y = i_Y;
	z = i_Z;
	w = i_W;
	assert(!isNAN(x));
	assert(!isNAN(y));
	assert(!isNAN(z));
	assert(!isNAN(w));
}

inline Engine::Vector3 Engine::Vector4::toVector3()
{
	return Vector3(x,y,z);
}
inline float Engine::Vector4::getx() const
{
	assert(!isNAN(x));
	return(x);
}

inline float Engine::Vector4::gety() const
{
	assert(!isNAN(y));
	return(y);
}

inline float Engine::Vector4::getz() const
{
	assert(!isNAN(z));
	return(z);
}

inline float Engine::Vector4::getw()const
{
	assert(!isNAN(w));
	return(w);
}

inline void Engine::Vector4::set(const float i_X, const float i_Y, const float i_Z, const float i_W)
{
	x = i_X;
	y = i_Y;
	z = i_Z;
	w = i_W;
	assert(!isNAN(x));
	assert(!isNAN(y));
	assert(!isNAN(z));
	assert(!isNAN(w));
}


inline void Engine::Vector4::setx(const float i_X)
{
	x = i_X;
	assert(!isNAN(x));
}

inline void Engine::Vector4::sety(const float i_Y)
{
	y = i_Y;
	assert(!isNAN(y));
}

inline void Engine::Vector4::setz(const float i_Z)
{
	z = i_Z;
	assert(!isNAN(z));
}

inline void Engine::Vector4::setw(const float i_W)
{
	w = i_W;
	assert(!isNAN(w));
}

inline const Engine::Vector4 Engine::Vector4::operator=(const Engine::Vector4& i_Vector)
{
	x = i_Vector.getx();
	y = i_Vector.gety();
	z = i_Vector.getz();
	w = i_Vector.getw();
	assert(!isNAN(x));
	assert(!isNAN(y));
	assert(!isNAN(z));
	assert(!isNAN(w));
	Engine::Vector4 temp = *this;
	return(temp);
}

inline const Engine::Vector4 Engine::Vector4::operator=(const float i_rhs)
{
	x = i_rhs;
	y = i_rhs;
	z = i_rhs;
	assert(!isNAN(x));
	assert(!isNAN(y));
	assert(!isNAN(z));
	Engine::Vector4 temp = *this;
	return(temp);
}


inline const Engine::Vector4 Engine::Vector4::operator+=(const Engine::Vector4& i_Vector)
{

	x = x + i_Vector.getx();
	y = y + i_Vector.gety();
	z = z + i_Vector.getz();
	Engine::Vector4 temp = *this;
	return(temp);
}

inline Engine::Vector4 operator+(const Engine::Vector4& lhs, const Engine::Vector4& rhs)
{
	Engine::Vector4 temp;
	temp.setx(lhs.getx() + rhs.getx());
	temp.sety(lhs.gety() + rhs.gety());
	temp.setz(lhs.getz() + rhs.getz());
	return temp;
}

inline Engine::Vector4 operator-(const Engine::Vector4& lhs, const Engine::Vector4& rhs)
{
	Engine::Vector4 temp;
	temp.setx(lhs.getx() - rhs.getx());
	temp.sety(lhs.gety() - rhs.gety());
	temp.setz(lhs.getz() - rhs.getz());
	return temp;
}

inline float Engine::operator*(const Engine::Vector4& lhs, const Engine::Vector4& rhs)
{
	return(lhs.getx()*rhs.getx()+lhs.gety()*rhs.gety()+lhs.getz()*rhs.getz());
}

inline bool Engine::operator==(const Engine::Vector4& lhs, const Engine::Vector4& rhs)
{
	if (!Engine::compareFloats(lhs.getx(), rhs.getx(), 5) || !Engine::compareFloats(lhs.getx(), rhs.getx(), 5) || !Engine::compareFloats(lhs.getx(), rhs.getx(), 5) || !Engine::compareFloats(lhs.getw(), rhs.getw(), 5))
		return false;
	else
		return true;
}

inline std::ostream& Engine::operator<<(std::ostream& i_out, const Engine::Vector4& rhs)
{
	i_out << "( " << rhs.getx() << " , " << rhs.gety() << " , " << rhs.getz() << " , "<<rhs.getw()<<")";
	return i_out;
}