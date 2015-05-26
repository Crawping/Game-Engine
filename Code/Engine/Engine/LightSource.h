#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "Vector3.h"
namespace Engine
{
	class LightSource
	{
		Vector3 m_direction;
		Vector3 m_color;
		Vector3 m_position;
	public:
		LightSource();
		LightSource(Vector3 i_direction, Vector3 i_color, Vector3 i_position) :m_direction(i_direction), m_color(i_color), m_position(i_position){};
		inline Vector3 getDirection() const;
		inline void setDirection(Vector3 i_direction);
		inline Vector3 getColor() const;
		inline void setColor(Vector3 i_direction);
		inline Vector3 getPosition() const;
		inline void setPosition(Vector3 i_direction);

	};
#include "LightSource.inl"
}
#endif // LIGHTSOURCE_H