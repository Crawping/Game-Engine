#include "DebugCheckBox.h"

Engine::DebugCheckBox::DebugCheckBox(std::string i_name, float i_screenPositionx, float i_screenPositiony,
	float i_width, float i_height, bool* i_value, void(*i_callbackFunction)(bool, void*), void* i_param) :m_name(i_name), m_value(i_value), m_callbackFunction(i_callbackFunction), m_param(i_param)
{
	Quad outerBox(i_screenPositionx - i_width / 2, i_screenPositiony - i_height / 3, i_width, i_height);
	Quad innerBox(i_screenPositionx - i_width / 3, i_screenPositiony - i_height / 2, 2 * i_width / 3, 2 * i_height / 3);
	m_outerBoxSprite.SetQuad(outerBox);
	m_innerBoxSprite.SetQuad(innerBox);
	if (*m_value == false)
	{
		m_innerBoxSprite.SetRenderable(false);
	}
}

void Engine::DebugCheckBox::GetBoxSprites(Engine::Sprite** o_spriteArray)
{
	o_spriteArray[0] = &m_outerBoxSprite;
	o_spriteArray[1] = &m_innerBoxSprite;
}

void Engine::DebugCheckBox::updateWidget(bool i_Increment, HWND i_mainWindow)
{
	if (*m_value)
	{
		*m_value = false;
		m_innerBoxSprite.SetRenderable(false);
		m_callbackFunction(false, m_param);
	}
	else
	{
		*m_value = true;
		m_innerBoxSprite.SetRenderable(true);
		m_callbackFunction(true, m_param);
	}
}