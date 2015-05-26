#ifndef DEBUGBUTTON_H
#define DEBUGBUTTON_H

#include "IDebugWidget.h"
#include "Sprite.h"
namespace Engine
{
	class DebugButton:public IDebugWidget
	{
		Sprite m_button;
		std::string m_name;
		void(*m_callbackFunction)(void);
	public:
		DebugButton(std::string i_name, float i_screenPositionx, float i_screenPositiony,
			float i_width, float i_height, void(*i_callbackFunction)(void));

		Sprite* GetButtonSprite();
		void updateWidget(bool i_Increment, HWND i_mainWindow);
	};
}
#endif //DEBUGBUTTON_H