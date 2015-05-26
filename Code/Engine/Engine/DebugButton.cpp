#include "DebugButton.h"

Engine::DebugButton::DebugButton(std::string i_name, float i_screenPositionx, float i_screenPositiony,
	float i_width, float i_height, void(*i_callbackFunction)(void)) : m_callbackFunction(i_callbackFunction)
{
	Quad buttonQuad(i_screenPositionx - i_width / 2, i_screenPositiony - i_height / 2, i_width, i_height);
	m_button.SetQuad(buttonQuad);
}

Engine::Sprite* Engine::DebugButton::GetButtonSprite()
{
	return &m_button;
}
void Engine::DebugButton::updateWidget(bool i_Increment, HWND i_mainWindow)
{
	m_callbackFunction();
}