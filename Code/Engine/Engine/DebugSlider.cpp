#include "DebugSlider.h"

Engine::DebugSlider::DebugSlider(std::string i_name, float i_screenPositionx, float i_screenPositiony,
	float i_width, float i_height, float* i_value,
	float i_minValue, float i_maxValue) :m_name(i_name), m_value(i_value), m_minValue(i_minValue), m_maxValue(i_maxValue),
	m_knobOffset((*m_value - m_minValue) / (m_maxValue - m_minValue)),
	m_sliderWidth(i_width), m_range(m_maxValue - m_minValue)
{
	m_sliderTopLeft = i_screenPositionx - i_width / 2;
	Quad sliderBaseQuad(i_screenPositionx - i_width / 2, i_screenPositiony - i_height / 2, i_width, i_height);
	Quad sliderKnobQuad(i_screenPositionx - i_width / 2 + m_knobOffset*i_width, i_screenPositiony - i_height / 2, i_height / 5, i_height);
	m_sliderBase.SetQuad(sliderBaseQuad);
	m_sliderKnob.SetQuad(sliderKnobQuad);
	m_sliderKnobQuad = m_sliderKnob.GetQuad();
}

void Engine::DebugSlider::GetSliderSprites(Engine::Sprite** o_spriteArray)
{
	o_spriteArray[0] = &m_sliderBase;
	o_spriteArray[1] = &m_sliderKnob;
}

Engine::Sprite* Engine::DebugSlider::GetSliderKnobSprite()
{
	return &m_sliderKnob;
}

void Engine::DebugSlider::updateWidget(bool i_Increment, HWND i_mainWindow)
{
	if (i_Increment)
	{
		*m_value += (m_range) / 10;
		if (*m_value >= m_maxValue)
			*m_value = m_maxValue;
	}
	else
	{
		*m_value -= (m_range) / 10;
		if (*m_value <= m_minValue)
			*m_value = m_minValue;
	}
	m_knobOffset = (*m_value - m_minValue) / (m_maxValue - m_minValue);
	m_sliderKnobQuad->setTopLeftX(m_sliderTopLeft + m_knobOffset*m_sliderWidth);
	m_sliderKnob.FillQuadMeshBuffer(i_mainWindow);
}