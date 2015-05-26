#ifndef DEBUGSLIDER_H 
#define DEBUGSLIDER_H
#include "Quad.h"
#include "Sprite.h"
#include "Material.h"
#include "IDebugWidget.h"
#include <string>
namespace Engine
{
	class DebugSlider:public IDebugWidget
	{
		Sprite m_sliderBase;
		Sprite m_sliderKnob;
		std::string m_name;
		float* m_value;
		float m_sliderTopLeft;
		float m_sliderWidth;
		float m_minValue;
		float m_maxValue;
		float m_knobOffset;
		Quad* m_sliderKnobQuad;
		float m_range;
		
	public:

		DebugSlider(std::string i_name, float i_screenPositionx, float i_screenPositiony,
			float i_width, float i_height, float* i_value,
			float i_minValue, float i_maxValue);

		void GetSliderSprites(Sprite** o_spriteArray);
		Sprite* GetSliderKnobSprite();
		void updateWidget(bool i_Increment, HWND i_mainWindow);
	};
}
#endif //DEBUGSLIDER_H