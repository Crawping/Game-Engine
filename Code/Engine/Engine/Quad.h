#ifndef QUAD_H
#define QUAD_H

namespace Engine
{
	class Quad
	{
		float m_topLeftx;
		float m_topLefty;
		float m_width;
		float m_height;
	public:
		Quad(){};
		Quad(float i_topLeftx, float i_topLefty, float i_width, float i_height) :m_topLeftx(i_topLeftx), m_topLefty(i_topLefty),
																				m_width(i_width), m_height(i_height){};


		// GETTERS
		inline float getTopLeftX(){ return m_topLeftx; };
		inline float getTopLeftY(){ return m_topLefty; };
		inline float getWidth(){ return m_width; };
		inline float getHeight(){ return m_height; };

		// SETTERS

		inline void setTopLeftX(float i_topLeftX){ m_topLeftx = i_topLeftX; };
		inline void setTopLeftY(float i_topLeftY){ m_topLefty = i_topLeftY; };
		inline void setWidth(float i_width){ m_width = i_width; };
		inline void setHeight(float i_height){ m_height = i_height; };

	};
}
#endif //QUAD_H